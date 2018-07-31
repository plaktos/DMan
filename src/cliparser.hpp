#ifndef H_CLIPARSER
#define H_CLIPARSER

#include <iostream>
#include <string.h>

#ifndef T_HAS_RESERVE
#define T_HAS_RESERVE
/* Check if container T has reserve() 
See:
https://stackoverflow.com/questions/19392361/adding-element-to-back-of-stl-container
https://stackoverflow.com/questions/14882588/correct-signature-of-detect-presence-of-containerreserve
https://en.cppreference.com/w/cpp/types/enable_if
*/

template<typename T, typename = void>
struct has_reserve : std::false_type {;};

template<typename T>
struct has_reserve<T,
    std::enable_if_t<std::is_same<
            decltype(std::declval<T &>().reserve( std::declval<typename T::size_type &>())),
            void
        >::value
       > > : std::true_type {;};

#endif // T_HAS_RESERVE 


template<typename T>
class CliParser{
    public:
        CliParser() : m_flag_continue(0), m_flag_threads(0) {;}
        T parse(const int argc, const char * const * const argv);

        CliParser(const CliParser&) = delete;
        CliParser(CliParser&&) = delete;
        CliParser& operator=(const CliParser&) = delete;

        bool flag_continue(){return m_flag_continue;}
        bool flag_threads(){return m_flag_threads;}
    private:


        // Solved enable_if overloading not working inside the class
        // https://stackoverflow.com/questions/11248173/enable-if-seems-to-work-outside-a-class-but-not-inside
        // + irc http://coliru.stacked-crooked.com/a/763d66c70019d4c4
        // alternative c++17:
        //      void opt_reserve(T &t) { if
        //          constexpr(has_reserve<T>::value) {
        //          }
        //      }

        // Dummy reserve if the container doesnt support reserve
        template<typename U>
        void opt_reserve(U& /*_*/, std::size_t /*_*/,
                    std::enable_if_t<!has_reserve<U>::value, int> dummy = {} )
            {;} 

        // Reserve function if the container supports it
        template<typename U>
        void opt_reserve(U& u, std::size_t s,
                    std::enable_if_t<has_reserve<U>::value, int> dummy = {})
            { 
                u.reserve(u.size() + s); }

        bool m_flag_continue;
        bool m_flag_threads;
};

template<typename T>
T CliParser<T>::parse(const int argc, const char * const * const argv){
    T container;
    opt_reserve<T>(container, argc - 1);
    for(int i = 1; i < argc; ++i){
        if(strcmp(argv[i], "-c")==0){ m_flag_continue = true; continue; }
        if(strcmp(argv[i], "-t")==0){ m_flag_threads = true; continue; }
        container.insert( container.end(), typename T::value_type(argv[i]));
    }
    return container;
}

#endif // H_CLIPARSER
