#include "fs/i_fs.h"
#include "fs/i_fs_node.h"
#include "fs/dev_fs.h"
#include "fs/fat_fs.h"
#include "logger/logger.h"

#include <string>

namespace iposix {
namespace fs {

::std::map< ::std::string, i_fs_node_ptr > i_fs::open_nodes;

void i_fs::open( i_fs_node_ptr node )
{
	i_fs::i_fs_dump();

	if ( node )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": marking new node opened " << node->path.c_str() << endl;

		if ( node->path.empty() )
		{
			logger << message::ERROR << __func__
				<< ": Could not open a node with no path" << endl;
		}
		else
		{
			i_fs::open_nodes[node->path] = node;
			node->uses++;
		}
	}
	else
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": got a null pointer" << endl;
	}

	i_fs::i_fs_dump();
}

void i_fs::close_node( i_fs_node_ptr node )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": closing node " 
		<< node->path.c_str()
		<< endl;

	i_fs::i_fs_dump();

	if ( node )
	{
		//if this is not a root node or the last use is in the mount map
		if ( node->uses <= 1 )
		{
			auto it = i_fs::open_nodes.find( node->path );
			if ( it->second )
			{
				i_fs::open_nodes.erase( it );
				logger << message::MORE_VERBOSE << __func__
					<< ": node closed normally" << endl;
			}
		}
		else
		{
			logger << message::MORE_VERBOSE << __func__
				<< ": won't close cause it's opened elsewhere" << endl;

			node->uses--;
		}
	}
	else
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": got a null pointer" << endl;
	}

	i_fs::i_fs_dump();
}

i_fs_node_ptr i_fs::open_subnode(
		i_fs_node_ptr node,
		const ::std::string& fullname,
		const ::std::string& subname )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Answering request for " << fullname.c_str()
		<< " with subname " << subname.c_str()
		<< endl;

	//check open nodes
	i_fs_node_ptr ret = i_fs::open_nodes.find( fullname )->second;
	if ( ret )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Found an open node" << endl;
		return ret;
	}

	// check for mount points
	ret = i_fs::mounted_filesystems.find( fullname )->second;
	if ( ret )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Found a mount point" << endl;
		return ret;
	}

	if ( subname.empty() )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": No open/mount node found returning input node" << endl;
		return node;
	}

	if ( node && node->is_directory )
	{
		// last chance get a new node
		ret = i_fs_node_ptr(
				node->open_subnode( fullname, subname )
				);
		if ( ret )
		{
			logger << message::MORE_VERBOSE << __func__
				<< ": Found a subnode" << endl;
			return ret;
		}
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Nothing found" << endl;

	return ret;
}

i_fs_node_ptr i_fs::open_node(
		const ::std::string& name
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Answering request for " << name.c_str() << endl;

	i_fs_node_ptr base;
	bool separator = false;
	::std::string fullname;
	::std::vector< ::std::string > tokens = i_fs::clean_path( name );

	if ( tokens[0] == "/" )
	{
		//absoulte addressing
		base = i_fs::mounted_filesystems.find( "/" )->second;
		fullname = "/";
		tokens.erase( tokens.begin() );
		separator = false;
	}
	else
	{
		//TODO relative addressing
		//get current working directory
		logger << message::ERROR << __func__
			<< ": Relative addressing not implemented" << endl;
		return i_fs_node_ptr();
	}

	if ( !base )
	{
		logger << message::ERROR << __func__
			<< ": could not get root node" << endl;
		return i_fs_node_ptr();
	}

	for ( auto it = tokens.begin(); base && it != tokens.end(); it++ )
	{
		if ( separator )
		{
			fullname.push_back( '/' );
		}
		else
		{
			separator = true;
		}
		fullname.append( *it );

		logger << message::MORE_VERBOSE << __func__
			<< ": fullname = " << fullname.c_str() << endl;
		logger << message::MORE_VERBOSE << __func__
			<< ": subname = " << it->c_str() << endl;

		base = i_fs::open_subnode( base, fullname, *it );

	}

	if ( base )
	{
		i_fs::open( base );
	}

	return base;
}

::std::vector< ::std::string > i_fs::clean_path( const ::std::string& path )
{
	// this method has a special feature or bug:
	// cd "..." becomes cd "../."
	// cd "...." becomes cd "../.."
	// nice isn't it - easy to fix just ignore dots after ".."

	::std::vector< ::std::string > tokens; //our hunted tokens
	::std::string token; //temporary token

	uint32_t deletable_tokens = 0; //count the deletable tokens

	// okay lets make tokens separated by '/'
	bool first_token = true;
	for (
			::std::string::const_iterator it = path.begin();
			it != path.end();
			it++
		)
	{
		switch ( *it )
		{
			case '/':
				if ( first_token )
				{
					//only accept slashes as the first token
					if ( token.empty() )
					{
						// so out a good one in
						token.push_back( '/' );
					}
				}
				//again we dont want single dots (but the first token) or empty tokens
				if ( !token.empty() && ( first_token || token != "." ) )
				{
					if ( token != "/" )
					{
						//we can't delete the leading slash
						deletable_tokens++;
					}
					// this should be a good token so take it
					tokens.push_back( token );
					first_token = false; // no more leading slashes
				}
				token.clear(); //let's make ready for a new token
				break;

			case '.':
				token.push_back( '.' );
				if ( token == ".." )
				{
					// double dots means go back in hierachie
					if ( deletable_tokens > 0 )
					{
						// so delete the last token if there is one deletable
						tokens.pop_back();
						deletable_tokens--;
					}
					else
					{
						// hm okay nothing to delete so put it in
						tokens.push_back( token );
						first_token = false; // no more leading slashes
					}
					token.clear(); //lets make ready for a new token
				}
				break;

			default:
				// puh thats easy a normal char ;) - i think
				token.push_back( *it );
		}
	}

	// because of the termination condition of the loop the last token is not
	// taken, so do it here if there is one
	if ( !token.empty() )
	{
		tokens.push_back( token );
	}

	return tokens;
}

void i_fs::i_fs_init()
{
	i_fs::mounted_filesystems.clear();
	i_fs::open_nodes.clear();
}

void i_fs::i_fs_dump()
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Dumping mount_points "
		<< i_fs::mounted_filesystems.size()
		<< endl;
	
	for (
			auto it = i_fs::mounted_filesystems.begin();
			it != i_fs::mounted_filesystems.end();
			++it
		)
	{
		logger << '\t' << it->first.c_str() << endl;
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Dumping open_nodes " 
		<< i_fs::open_nodes.size()
		<< endl;
	
	for (
			auto it = i_fs::open_nodes.begin();
			it != i_fs::open_nodes.end();
			++it
		)
	{
		logger << '\t' << it->first.c_str() << endl;
	}
}

} //namespace fs
} //namespace iposix
