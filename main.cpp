#include "app.hpp"
#include <cppx-core/_all_.hpp>          // https://github.com/alf-p-steinbach/cppx-core
$use_cppx( description_lines_from, Raw_array_, monospaced_bullet_block );
$use_std( vector, string, exception, cerr, endl );

auto main( const int n_args, const Raw_array_<char*> args )
    -> int
{
    try
    {
        app::run_with_args( vector<string>( args + 1, args + n_args ) );
        return EXIT_SUCCESS;
    }
    catch( const exception& x )
    {
        const string text = description_lines_from( x );
        cerr << monospaced_bullet_block( text, "!" ) << endl;
    }
    return EXIT_FAILURE;
}
