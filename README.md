Download Visual Studio 2022

When Installing, select Desktop Development with C++ and Universal Windows Platform development
(image)

Once Visual Studio 2022 is installed, download the files and extract them in a folder

Open the "Sources" folder and open helbreath-equilibrium-enhanced.sln
(image)

At the top of Visual Studio, make sure that "Release" is selected instead of "Debug"

Press Ctrl + Shift + B to Build the Client.exe and HGserver.exe

Go to the "helbreath-equilibrium-enhanced\Server\" folder, and set your local IP in GateServer.cfg, Core/HMLServer.cfg and Core/WLserver.cfg

Go to "helbreath-equilibrium-enhanced\Server\GameServers\all" and set the IP for "gserver.cfg". Make sure to set the "game-server-external-address" correctly, which is different from the internal IP. You can check your external IP at https://whatismyip.com 

Open HGserver.exe
Open GateServer.exe
Open MainServer.exe
Open WorldServer.exe
Activate MainServer
Activate WorldServer

Go to "helbreath-equilibrium-enhanced\Client\Helbreath\", copy "LOGIN.CFG.tpl", and rename the copy to "LOGIN.CFG"

Read the instructions in LOGIN.CFG for LAN or INTERNET
Open Client.exe

Account: master
Password: asd

Have fun!