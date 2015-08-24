Proxxi Login Client
------------
----------
This application is an authenticator and user session manager for users within the Proxxi and IX domain.
By logging in, you are granted internet access as well as access to various local resources like network drives and printers.
When you log out, your session will be clensed from the computer so that the next user can get a clean experience.

The application was designed to improve the experience people had at a non-profit association called "Proxxi", where the IT management was managed by a non-profit association called "IX".
Since "Proxxi" closed in february 2015, development has been on halt with only minor fixes up until this point.

This application was my first serious attempt at learning C++.
It is cross plattform(Windows, Linux, OS X) and built using cross plattform libraries such as wxWidgets and CURL.
It is developed by me alone, and copyrighted as such under the Creative Commons "by-nc-nd" license.

----------
Requirements
------------
----------
Windows
 - Windows 7 or newer
 - Microsoft Visual C++ Redistributable Packages for Visual Studio 2013

Linux
 - Any GTK3 compatible desktop with full Unicode UTF-8 support
 - wxWidgets libraries (wx3.0-headers, wx-common, libwxbase3.0-dev, libwxgtk3.0-dev)
 - CURL libraries (curl, libcurl4-openssl-dev)

OS X
 - OS X 10.5 or newer

----------
Usage
-----
----------
Simply log in and you will be greeted with internet access and access to local network resources.
When you log out, your session data will be clensed.


Screenshot from Windows 8:
![Program Preview](http://files.erikwelander.se/git/images/Proxxi_Login_Client/Proxxi_Login_Client.png)

----------
