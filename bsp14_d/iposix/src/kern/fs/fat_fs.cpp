#include "fs/fat_fs.h"
#include "fs/fat_fs_node.h"
#include "fs/fat_directory_entry.hpp"
#include "logger/logger.h"

namespace iposix {
namespace fs {

void fat_bootsector_t::dump( fat_type_t as_type ) const
{
	//some marcos for easier use
	#define dmp( name, value ) \
		logger << message::MORE_VERBOSE << '\t' << #name << ":=\"" \
		<< value << "\"" << endl

	#define dmp_str( value ) \
		dmp( value , &this->value[0] )

	#define dmp_uns( value ) \
		dmp( value, hex << static_cast<uint8_t>( this->i.value) )

	#define dmp_hex( value ) \
		dmp( value, hex << this->value )

	#define dmp_dec( value ) \
		dmp( value, dec << this->value )
		
	switch ( as_type )
	{
		case fat12:
			logger << message::MORE_VERBOSE << "fat12 bootsector:" << endl;
			break;

		case fat16:
			logger << message::MORE_VERBOSE << "fat16 bootsector:" << endl;
			break;

		case fat32:
			logger << message::MORE_VERBOSE << "fat32 bootsector:" << endl;
			break;

		default:
			break;
	}

	dmp_str( oem_name );
	dmp_dec( bytes_per_sector );
	dmp_dec( sector_per_cluster );
	dmp_dec( reserved_sectors );
	dmp_dec( fat_copies );
	dmp_dec( max_dir_entrys_in_root );
	dmp_dec( sectors_old );
	dmp_hex( media_descriptor );
	dmp_dec( sectors_per_fat );
	dmp_dec( sectors_per_track );
	dmp_dec( pages_or_heads );
	dmp_dec( hidden_sectors );
	dmp_dec( sectors );

	switch ( as_type )
	{
		case fat12:
			dmp_hex( fat12_info.physical_drive_number );
			dmp_hex( fat12_info.ext_bootsignature );
			dmp_hex( fat12_info.serial_number );
			dmp_str( fat12_info.label );
			dmp_str( fat12_info.fat_variant );
			break;

		case fat16:
			dmp_hex( fat16_info.physical_drive_number );
			dmp_hex( fat16_info.ext_bootsignature );
			dmp_hex( fat16_info.serial_number );
			dmp_str( fat16_info.label );
			dmp_str( fat16_info.fat_variant );
			break;

		case fat32:
			dmp_dec( fat32_info.sectors_per_fat );
			dmp_hex( fat32_info.fat_flags );
			dmp_hex( fat32_info.fat_32_version );
			dmp_dec( fat32_info.rootdir_cluster );
			dmp_dec( fat32_info.fs_information_sector );
			dmp_dec( fat32_info.bootsector_copy_sector );
			dmp_hex( fat32_info.physical_drive_number );
			dmp_hex( fat32_info.ext_bootsignature );
			dmp_hex( fat32_info.serial_number );
			dmp_str( fat32_info.label );
			dmp_str( fat32_info.fat_variant );
			break;

		default:
			break;
	}

	dmp_hex( bios_bootsignature );

	//throw the defines away
	#undef dmp_dec
	#undef dmp_hex
	#undef dmp_uns
	#undef dmp_str
	#undef dmp
}

clustertable_t::clustertable_t( const fat_type_t& type )
	: data(),
	size_bytes( 0 ),
	add( 0 ),
	type( type )
{
	if ( sizeof( clustertable_data_t ) != clustertable_t::BYTES )
	{
		logger << message::ERROR
			<< __func__ << "clustertable_data_t has wrong size (" 
			<< sizeof( clustertable_data_t ) << "!="
			<< clustertable_t::BYTES
			<< ")" << endl;
	}
}

clustertable_t::~clustertable_t()
{
	if ( this->data )
	{
		delete this->data;
		this->data = 0;
	}
}

void clustertable_t::size(const uint32_t size)
{
	this->size_bytes = size;

	if ( this->data )
	{
		delete this->data;
		this->data = 0;
	}

	uint8_t* ptr = new uint8_t[size];
	this->data = reinterpret_cast<clustertable_data_t*>(ptr);
}

uint32_t clustertable_t::size() const
{
	return this->size_bytes;
}

void clustertable_t::norm_node_add( uint32_t add )
{
	this->add = add;
}

uint32_t clustertable_t::value(uint32_t cluster) const
{
	uint32_t ret = 0;
	if ( this->data )
	{
		switch ( this->type )
		{
			case fat12:
				{
					uint32_t entry_number = cluster / FAT12_ENTRYS;
					switch ( cluster % FAT12_ENTRYS )
					{
						case 0:
							ret = this->data[entry_number].fat12_entry.value0;
							break;

						case 1:
							ret = this->data[entry_number].fat12_entry.value1;
							break;

						case 2:
							ret = this->data[entry_number].fat12_entry.value2;
							break;

						case 3:
							ret = this->data[entry_number].fat12_entry.value3;
							break;

						case 4:
							ret = this->data[entry_number].fat12_entry.value4;
							break;

						case 5:
							ret = this->data[entry_number].fat12_entry.value5;
							break;

						case 6:
							ret = this->data[entry_number].fat12_entry.value6;
							break;

						case 7:
							ret = this->data[entry_number].fat12_entry.value7;
							break;
					}
				}
				break;

			case fat16:
				{
					uint32_t entry_number = cluster / FAT16_ENTRYS;
					uint32_t value_number = cluster % FAT16_ENTRYS;

					ret = this->data[entry_number].fat16_entry[value_number];
				}
				break;

			case fat32:
				{
					uint32_t entry_number = cluster / FAT32_ENTRYS;
					uint32_t value_number = cluster % FAT32_ENTRYS;

					ret = this->data[entry_number].fat32_entry[value_number];
				}
				break;
		}
	}
	return ret;
}

uint32_t clustertable_t::successor( uint32_t cluster ) const
{
	cluster = cluster - add + 2;

	uint32_t value = this->value(cluster);
	if ( this->has_successor( cluster ) )
	{
		//TODO throw exception on nonexisten successor
	}

	return value + add - 2 ;
}

uint32_t clustertable_t::chain_length( uint32_t cluster ) const
{
	uint32_t length( 0 );
	
	if ( !( is_free( cluster ) || is_defect( cluster ) ) )
	{
		length = 1;
	}

	while ( has_successor( cluster ) )
	{
		cluster = this->successor( cluster );
		length++;
	}

	return length;
}

bool clustertable_t::has_successor( uint32_t cluster ) const
{
	return !( is_free( cluster ) || is_defect( cluster ) || is_end( cluster ) );
}

bool clustertable_t::is_free( uint32_t cluster ) const
{
	cluster += 2;
	uint32_t value = this->value(cluster);
	switch ( type )
	{
		case fat12:
			return ( value == FAT12_FREE );

		case fat16:
			return ( value == FAT16_FREE );

		case fat32:
			return ( value == FAT32_FREE );

		default:
			return false;
	}
}

bool clustertable_t::is_defect( uint32_t cluster ) const
{
	cluster += 2;
	uint32_t value = this->value(cluster);
	switch ( type )
	{
		case fat12:
			return ( value == FAT12_DEFECT );

		case fat16:
			return ( value == FAT16_DEFECT );

		case fat32:
			return ( value == FAT32_DEFECT );

		default:
			return true;
	}
}

bool clustertable_t::is_end( uint32_t cluster ) const
{
	cluster += 2;
	uint32_t value = this->value(cluster);
	switch ( type )
	{
		case fat12:
			return ( value >= FAT12_END );

		case fat16:
			return ( value >= FAT16_END );

		case fat32:
			return ( value >= FAT32_END );

		default:
			return false;
	}
}

void clustertable_t::dump() const
{
	if ( !this->data )
	{
		return;
	}

	uint32_t clusters = 0;

	switch ( this->type )
	{
		case fat12:
			clusters = ( this->size_bytes * FAT12_ENTRYS ) / BYTES;
			break;

		case fat16:
			clusters = this->size_bytes >> 1;
			break;

		case fat32:
			clusters = this->size_bytes >> 2;
			break;
	}

	uint32_t free = 0;
	uint32_t non_continous = 0;
	uint32_t defect = 0;

	uint32_t last_value = 0;
	for ( uint32_t i = 0; i < clusters; ++i )
	{
		uint32_t value = this->value( i );
		if ( this->is_free( i ) )
		{
			++free;
		}
		else if ( this->is_defect( i ) )
		{
			++defect;
		}
		else if ( last_value > value )
		{
			++non_continous;
		}
		last_value = value;
	}

	logger << message::MORE_VERBOSE
		<< "Fat Clustertable Statistic:"
		<< " Free cluster: " << free
		<< " Non-continous cluster: " << non_continous
		<< " Defect cluster: " << defect
		<< endl;
}

uint32_t fat_directory_entry_t::cluster( const fat_type_t type ) const
{
	uint32_t ret( this->fat32_cluster_low );
	
	switch ( type )
	{
		case fat32:
			ret |= ( this->fat32_cluster_high << 16 );
			break;

		default:
			break;
	}

	return ret;
}

void fat_directory_entry_t::dump() const
{
	//some marcos for easier use
	#define dmp( value ) \
		logger << message::MORE_VERBOSE << value

	#define dmp_str( value ) \
		dmp( &this->value[0] )

	#define dmp_uns( value ) \
		dmp( dec << static_cast<uint8_t>( this->value) )

	#define dmp_hex( value ) \
		dmp( hex << this->value )

	#define dmp_dec( value ) \
		dmp( dec << this->value )

	#define dmp_test( value, letter ) \
		logger << message::MORE_VERBOSE << ( value ? letter : '-' )

	
	dmp_test( attributes.write_protected, 'r');
	dmp_test( attributes.hidden, 'h' );
	dmp_test( attributes.system, 's' );
	dmp_test( attributes.volume_label, 'v' );
	dmp_test( attributes.directory, 'd' );
	dmp_test( attributes.archive, 'a' );

	dmp_dec( size );
	dmp( 'B' );
	dmp( ' ' );

	dmp_uns( create_date.year );
	dmp( '.' );
	dmp_uns( create_date.month );
	dmp( '.' );
	dmp_uns( create_date.day );

	dmp( ' ' );

	dmp_uns( create_time.hour );
	dmp( ':' );
	dmp_uns( create_time.minute );
	dmp( ':' );
	dmp_uns( create_time.second() );
	//dmp_dec( creation_time_millis_div10 );

	dmp( ' ' );

	dmp_uns( last_access_date.year );
	dmp( '.' );
	dmp_uns( last_access_date.month );
	dmp( '.' );
	dmp_uns( last_access_date.day );

	dmp( ' ' );

	dmp_uns( last_modification_date.year );
	dmp( '.' );
	dmp_uns( last_modification_date.month );
	dmp( '.' );
	dmp_uns( last_modification_date.day );

	dmp( ' ' );

	dmp_uns( last_modification_time.hour );
	dmp( ':' );
	dmp_uns( last_modification_time.minute );
	dmp( ':' );
	dmp_uns( last_modification_time.second() );

	dmp( ' ' );

	logger << this->filename().c_str();

	dmp( ' ' );

	dmp_hex( fat32_cluster_high );
	dmp_hex( fat32_cluster_low );

	logger << endl;

	//throw the defines away
	#undef dmp_dec
	#undef dmp_hex
	#undef dmp_uns
	#undef dmp_str
	#undef dmp
}

const ::std::string fat_directory_entry_t::filename() const
{
	/**
	 * true if not null and not space
	 * @param[in] value the value to check
	 */
	#define char_ok( value ) \
		( value != '\0' ) && ( value != ' ' )
	uint32_t length;

	//check basename length
	for ( 
			length = 0; 
			( length < 8 ) && char_ok( this->name[length] );
			length++ 
		);

	//append base name
	::std::string filename( &this->name[0], length );

	//check extension length
	for ( length = 0; 
			( length < 3 ) && char_ok( this->extension[length] );
			length++ );

	//append extension if there is one
	if ( length > 0 )
	{
		filename.append(".");
		filename.append( &this->extension[0], length );
	}

	//make lower case
	for ( 
			auto it = filename.begin();
			it != filename.end();
			it++
		)
	{
		if ( *it >= 'A' && *it <= 'Z' )
		{
			*it += ('a' - 'A' );
		}
	}

	return filename;
}

fat_fs::fat_fs(
		i_fs_node_ptr container,
		fat_type_t type
		)
	: i_fs( container ),
	clustertable( type ),
	type( type ),
	fat_bootsector()
{ }

fat_fs::~fat_fs()
{ }

i_fs_node* fat_fs::fs_root_node( const ::std::string& path )
{
	uint32_t cluster = 0;

	switch ( this->type )
	{
		case fat12:
		case fat16:
			cluster = 0;
			break;

		case fat32:
			cluster = this->fat_bootsector.fat32_info.rootdir_cluster;
			break;
	}

	fat_fs_ptr fs = fat_fs_ptr( this );

	fat_directory_entry_t fake_entry;
	fake_entry.name[0] = ' ';
	fake_entry.name[1] = ' ';
	fake_entry.name[2] = ' ';
	fake_entry.name[3] = ' ';
	fake_entry.name[4] = ' ';
	fake_entry.name[5] = ' ';
	fake_entry.name[6] = ' ';
	fake_entry.name[7] = ' ';
	fake_entry.extension[0] = ' ';
	fake_entry.extension[1] = ' ';
	fake_entry.extension[2] = ' ';
	fake_entry.attributes.write_protected = 0;
	fake_entry.attributes.hidden = 0;
	fake_entry.attributes.system = 1;
	fake_entry.attributes.volume_label = 0;
	fake_entry.attributes.directory = 1;
	fake_entry.attributes.archive = 1;
	fake_entry.fat32_cluster_high = cluster >> 16;
	fake_entry.fat32_cluster_low = cluster & 0xFF;

	return new fat_fs_node( fs, path, true, true, cluster, fake_entry );
}

uint32_t fat_fs::cluster_size() const
{
	return ( 
			this->fat_bootsector.bytes_per_sector *
			this->fat_bootsector.sector_per_cluster
		   );
}

uint32_t fat_fs::norm_node_cluster_add() const
{
	uint32_t ret( 0 );

	// i think only fat12/16 needs this
	switch ( this->type )
	{
		case fat12:
		case fat16:
			ret = (
					(
					 this->fat_bootsector.max_dir_entrys_in_root * 
					 sizeof( fat_directory_entry_t ) 
					) / 
					( 
					 this->cluster_size() 
					)
				  );
			break;

		default:
			break;
	}

	return ret;
}

fat_type_t fat_fs::fat_type() const
{
	return this->type;
}

void fat_fs::init()
{
	this->read_bootsector();
	this->fat_bootsector.dump( this->type );

	this->read_clustertable();
	this->clustertable.dump();
}

void fat_fs::sync()
{
}

void fat_fs::read_bootsector()
{
	this->read( 0, &this->fat_bootsector, sizeof( fat_bootsector ) );
}

void fat_fs::read_clustertable()
{
	uint32_t cl_tab_size = 
		(
		 fat_bootsector.sectors_per_fat *
		 fat_bootsector.bytes_per_sector
		);
	uint32_t cl_tab_base =
		(
		 fat_bootsector.reserved_sectors *
		 fat_bootsector.bytes_per_sector
		);

	this->clustertable.size( cl_tab_size );

	this->read( cl_tab_base, &this->clustertable.data[0], cl_tab_size );

	this->clustertable.norm_node_add( this->norm_node_cluster_add() );
}

void fat_fs::read_cluster(
		const uint32_t cluster,
		void* buffer
		)
{
	uint32_t base_sector = 
		(
		 this->fat_bootsector.reserved_sectors + 
		 this->fat_bootsector.sectors_per_fat *
		 this->fat_bootsector.fat_copies
		);
	uint32_t base_cluster =
		(
		 base_sector / this->fat_bootsector.sector_per_cluster
		);

	uint64_t base = (base_cluster + cluster ) * this->cluster_size();

	uint32_t length( this->cluster_size() );

	this->read( base, buffer, length );
}

void fat_fs::single_cluster_io(
		fat_io_type_t io_type,
		uint32_t cluster,
		uint32_t offset,
		void* buffer,
		uint32_t length )
{
	if ( offset + length > this->cluster_size() )
	{
		return;
	}

	uint32_t base_sector = 
		(
		 this->fat_bootsector.reserved_sectors + 
		 this->fat_bootsector.sectors_per_fat *
		 this->fat_bootsector.fat_copies
		);
	uint32_t base_cluster =
		(
		 base_sector / this->fat_bootsector.sector_per_cluster
		);

	uint64_t base = (base_cluster + cluster ) * this->cluster_size();
	base += offset;

	switch ( io_type )
	{
		case cluster_read:
			this->read( base, buffer, length );
			break;

		case cluster_write:
			this->write( base, buffer, length );
			break;
	}
}

void fat_fs::cluster_io(
		fat_io_type_t io_type,
		uint32_t start_cluster,
		uint64_t offset,
		void* buffer,
		uint32_t length )
{
	uint32_t clustersize( this->cluster_size() );

	uint32_t skip_clusters( offset / clustersize );
	uint32_t skip_bytes( offset % clustersize );
	uint32_t cluster( start_cluster );

	for ( ; skip_clusters > 0; skip_clusters--)
	{
		if ( this->clustertable.has_successor( cluster ) )
		{
			cluster = this->clustertable.successor( cluster );
		}
		else
		{
			//TODO throw exception
			return;
		}
	}

	uint8_t* buf = reinterpret_cast<uint8_t*>(buffer);
	uint32_t remain( length );
	for ( uint32_t position( 0 ); position < length; )
	{
		uint32_t max_len( clustersize - skip_bytes );
		uint32_t len( remain > max_len ? max_len : remain );
		this->single_cluster_io(
				io_type, cluster, skip_bytes, &buf[position], len );
		skip_bytes = 0;

		remain -= len;
		position += len;
		
		if ( remain > 0 && this->clustertable.has_successor( cluster ) )
		{
			cluster = this->clustertable.successor( cluster );
		}
		else
		{
			//TODO throw exception
			return;
		}
	}
}


} //namespace fs
} //namespace iposix
