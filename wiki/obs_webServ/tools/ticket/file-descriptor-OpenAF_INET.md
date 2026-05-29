Warning : Reçu SIGINT (Ctrl+C), arrêt du serveur... 
Error: poll() failed (interrupted system call?)
Success : close socket 
Success : close socket 
Success : close socket 
==889569== 
==889569== FILE DESCRIPTORS: 4 open (3 inherited) at exit.
==889569== Open AF_INET socket 6: 127.0.0.1:8080 <-> 127.0.0.1:60232
==889569==    at 0x4CFABDD: accept (accept.c:26)
==889569==    by 0x408BDF: SocketHandler::acceptConnection(int, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&, int&) (in /home/bchallat/curr-workrep/webServer/webserv)
==889569==    by 0x40C204: EventLoop::acceptNewConnection(int) (in /home/bchallat/curr-workrep/webServer/webserv)
==889569==    by 0x400A95: handler_conection(EventLoop&) (in /home/bchallat/curr-workrep/webServer/webserv)
==889569==    by 0x400DEB: inf_loop(std::vector<int, std::allocator<int> >) (in /home/bchallat/curr-workrep/webServer/webserv)
==889569==    by 0x400926: main (in /home/bchallat/curr-workrep/webServer/webserv)
==889569== 
==889569== Open file descriptor 2: /dev/pts/2
==889569==    <inherited from parent>
==889569== 
==889569== Open file descriptor 1: /dev/pts/2
==889569==    <inherited from parent>
==889569== 
==889569== Open file descriptor 0: /dev/pts/2
==889569==    <inherited from parent>
==889569== 
==889569== 
==889569== HEAP SUMMARY:
==889569==     in use at exit: 0 bytes in 0 blocks
==889569==   total heap usage: 83 allocs, 83 frees, 97,159 bytes allocated
==889569== 
==889569== All heap blocks were freed -- no leaks are possible
==889569== 
==889569== For lists of detected and suppressed errors, rerun with: -s
==889569== ERROR SUMMARY: 4 errors from 4 contexts (suppressed: 0 from 0)

