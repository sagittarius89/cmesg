###############################################################################
# CMESG
###############################################################################
CMESG is simple terminal communicator build on sockets, written in c.

###############################################################################
# BUILD
###############################################################################

make all
    Builds whole application

make clean
    Removes the results of the previous build

make dependencies
    Installs dependencies

make target=client
    Builds only client side applications

make target=server
    builds only server side applications

###############################################################################
# LAUNCH
###############################################################################

SERVER

Application has server side and client side binaries. First You should launch
server side binary on server host machine. To do this, go to build directory 
and type:
./server.out

For now server works only on port 8889 so You must have this port unlocked in
your router, client, and server machine.

To stop server use CTRL + C.


CLIENT

When server is launched build application on client machine, then go to build
directory and launch client binary passing server host address and your chat 
nick name by args. For example:

./client.out -a 127.0.0.1 -n YourNickName

Usage: client [options]
  -a, --host       Setup server ip address
  -p, --port       Setup server port (default 8889)
  -n, --nick       Setup your nick name (max 15 symbols)
  -h, --help       Print help


###############################################################################
# MANUAL
###############################################################################

Client GUI consists with two parts. Top part is window where all messages 
prints. This window is read-only. Bottom part is text input. When user start 
type text will apear there. All text will be send to server after user will 
press RETURN key. There are also available commands. All commands starts with 
slash character. List of available will print after type /help command. To
quit type /quit.
