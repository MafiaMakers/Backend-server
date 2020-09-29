g++ -I"C:\Program Files\Java\jdk-9.0.4\include"^
 -I"..\..\..\..\out\production\RoomServerJava\Mafia\Network\CPP_sources\RoomServerSources"^
 -I"..\..\..\..\..\..\StructParser"^
 -I"..\..\..\..\..\Server"^
 -fPIC -D DONT_USE_QT^
 "..\..\..\..\..\Server\System\functions.cpp"^
 "..\..\..\..\..\Server\System\simplestring.cpp"^
 "..\..\..\..\..\Server\System\keygen.cpp"^
 "..\..\..\..\..\Server\System\tuple.cpp"^
 "..\..\..\..\..\Server\System\tuple.cpp"^
 "..\..\..\..\..\Server\Exceptions\*.cpp"^
 "..\..\..\..\..\Server\Database\user.cpp"^
 "..\..\..\..\..\Server\Database\chatmessage.cpp"^
 "..\..\..\..\..\Server\Database\chatsettings.cpp"^
 "..\..\..\..\..\Server\Gameplay\game_s.cpp"^
 "..\..\..\..\..\Server\userstatistics.cpp"^
 "..\..\..\..\..\Server\clientinfo.cpp"^
 "..\..\..\..\..\Server\System\serializer.cpp"^
 "..\..\..\..\..\Server\Network\crypto.cpp"^
 "MessageParser.cpp"^
 "Serializer.cpp"^
 -shared -o "..\..\..\..\out\production\RoomServerJava\Mafia\Network\CPP_LIB\Serializer.so"