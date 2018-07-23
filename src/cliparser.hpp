#ifndef H_CLIPARSER
#define H_CLIPARSER

#include "../include/CLI11.hpp"

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
        CliParser(const int argc, const char * const * const argv)
        : m_argc(argc), m_argv(argv)
        {;}

        T operator()() { return parse(m_argc, m_argv); }

    private:
        // Prevent copying by moving ctors to private
        CliParser(const CliParser& other);
        CliParser(CliParser&& other);
        CliParser& operator=(const CliParser& other);


        // Solved enable_if overloading not working inside the class
        // https://stackoverflow.com/questions/11248173/enable-if-seems-to-work-outside-a-class-but-not-inside
        // + irc http://coliru.stacked-crooked.com/a/763d66c70019d4c4
        // alternative c++17:
        //      void opt_reserve(T &t) { if
        //          constexpr(has_reserve<T>::value) {
        //          }
        //      }
        //
        // Dummy reserve if the container doesnt support reserve
        template<typename U>
        void opt_reserve(U& /*_*/, std::size_t /*_*/,
                    std::enable_if_t<!std::is_same<U,bool>::value, int> dummy = {} )
            {;} 

        // Reserve function if the container supports it
        template<typename U>
        void opt_reserve(U& t, std::size_t s,
                    typename std::enable_if_t<std::is_same<U,bool>::value, int> dummy = {})
            { t.reserve(t.size() + s); }

        T parse(const int argc, const char * const * const argv){
            //-TODO switch to CLI11 later
            T container;
            opt_reserve<T>(container, argc - 1);
            for(int i = 1; i < argc; ++i)
                container.insert( container.end(), typename T::value_type(argv[i]));

            return container;
        }

        const int m_argc;
        const char* const * const m_argv;
};


#endif // H_CLIPARSER
