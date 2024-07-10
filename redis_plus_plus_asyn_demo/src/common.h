#include <iostream>

using namespace std;

auto show_vec = []( const std::string& name, const auto& v ) {
    cout << name + " [ ";
    for ( const auto& e : v )
    {
        cout << e << ", ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_map = []( const std::string& name, const auto& m ) {
    cout << name << " [ ";
    for ( const auto& e : m )
    {
        cout << "(" << e.first << ", " << e.second << "), ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_map_17 = []( const std::string& name, const auto& m ) {
    cout << name << " [ ";
    for ( const auto& [ f, s ] : m )
    {
        cout << "(" << f << ", " << s << "), ";
    }
    cout << "]";
    cout << endl << endl;
};

auto show_opt_vec = []( const std::string& name, const auto& v ) {
    cout << name + " [ ";
    for ( const auto& e : v )
    {
        if ( e )
        {
            cout << *e << ", ";
        }
        else
        {
            cout << "null, ";
        }
    }
    cout << "]";
    cout << endl << endl;
};
