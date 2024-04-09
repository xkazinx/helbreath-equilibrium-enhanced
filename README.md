Download Visual Studio 2022

When Installing, select Desktop Development with C++ and Universal Windows Platform development
(image)

Once Visual Studio 2022 is installed, download the files and extract them in a folder

Open the "Sources" folder and open helbreath-equilibrium-enhanced.sln
(image)

At the top of Visual Studio, make sure that "Release" is selected instead of "Debug", and that "x86" is selected over "x64"

Press Ctrl + Shift + B to Build the Client.exe and HGserver.exe

Go to the "helbreath-equilibrium-enhanced\Server\" folder, and set your local IP in GateServer.cfg, Core/HMLServer.cfg and Core/WLserver.cfg
To know what your local IP is, press Windows Key + R, type "cmd" and press Enter. Type "ipconfig" and press Enter. The local IP is the one that says IPv4 Address.

Go to "helbreath-equilibrium-enhanced\Server\GameServers\all" and set the IP for "gserver.cfg". Make sure to set the "game-server-external-address" correctly, which is different from the internal IP. You can check your external IP at https://whatismyip.com 

Open HGserver.exe in "helbreath-equilibrium-enhanced\Server\GameServers\all"
Open GateServer.exe
Open MainServer.exe
Open WorldServer.exe
Activate MainServer
Activate WorldServer

Go to "helbreath-equilibrium-enhanced\Client\Helbreath\", copy "LOGIN.CFG.tpl", and rename the copy to "LOGIN.CFG"
Make sure windows is displaying file extensions to successfully rename "LOGIN.CFG.tpl" to "LOGIN.CFG", you can enable that with this information (Search for windows 11 if you have that Operative System): https://www.bing.com/search?pglt=673&q=show+extensions+windows+10

Open and follow this instructions:
- In "game-server-mode" use LAN if you are hosting and playing in the same pc, or, use INTERNET if you are connecting to an external server
- In "log-server-address", the IP must be the internal (local IP) for LAN, and external for INTERNET

Open Client.exe

Account: master
Password: asd

Have fun!
