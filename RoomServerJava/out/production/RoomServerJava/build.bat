del /f "CPP_LIB\Main.so"

g++^
 -I"C:\Program Files\Java\jdk-9.0.4\include"^
 -I"..\..\..\..\Server"^
 -I"CPP_SOURCES\RoomServerSources"^
 -fPIC -D DONT_USE_QT^
 "CPP_SOURCES\RoomServerSources\IncludeList.cpp"^
 "..\..\..\..\Server\System\functions.cpp"^
 "..\..\..\..\Server\System\simplestring.cpp"^
 "..\..\..\..\Server\System\keygen.cpp"^
 "..\..\..\..\Server\System\tuple.cpp"^
 "..\..\..\..\Server\Exceptions\*.cpp"^
 "..\..\..\..\Server\Database\user.cpp"^
 "..\..\..\..\Server\Database\chatmessage.cpp"^
 "..\..\..\..\Server\Database\chatsettings.cpp"^
 "..\..\..\..\Server\Gameplay\game_s.cpp"^
 "..\..\..\..\Server\userstatistics.cpp"^
 "..\..\..\..\Server\clientinfo.cpp"^
 "..\..\..\..\Server\System\serializer.cpp"^
 "..\..\..\..\Server\Network\crypto.cpp"^
 "..\..\..\..\Server\Network\mainservernetworker.cpp"^
 "CPP_sources\RoomServerSources\includemessagesender.cpp"^
 "CPP_sources\RoomServerSources\Summator.cpp"^
 "CPP_sources\RoomServerSources\Main.cpp"^
 -shared -o^
 "CPP_LIB\Main.so"
