// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketIOgame/Public/SocketIOManager.h"



#include "WebSocketsModule.h"
#include "IWebSocket.h"



USocketIOManager::USocketIOManager() :
    _mainSocket(nullptr)
{

}

USocketIOManager::~USocketIOManager()
{

    //UE_LOG(LogTemp, Warning, TEXT("USocketIOManager deconstructed............"));
   // DisconnectedFromServer();

}


void USocketIOManager::InitializeSocketIO()
{
    if(!   FModuleManager::Get().IsModuleLoaded("WebSockets") ){
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Green, TEXT("Load WebSockets module now...."));
        FModuleManager::Get().LoadModule("WebSockets");
    }


  // ensureAlwaysMsgf()
}

int32 USocketIOManager::ConnectToUrl(const FString &Url)
{
    GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Red, TEXT("Try connect to : %s " )   );

    _mainSocket =  FWebSocketsModule::Get().CreateWebSocket(*Url); // "ws://127.0.0.1:8080"

    ensureAlwaysMsgf(_mainSocket.IsValid() , TEXT("Create WebSocket Failed....")  );

    //Setup listener.......
    _mainSocket->OnConnected().AddLambda([]() -> void {
        // This code will run once connected.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Green, TEXT("OnConnected ok.... " )   );
    });

    _mainSocket->OnConnectionError().AddLambda([](const FString & Error) -> void {
        // This code will run if the connection failed. Check Error to see what happened.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Red, Error  );
    });

    _mainSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
        // This code will run when the connection to the server has been terminated.
        // Because of an error or a call to Socket->Close().
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Red, TEXT("OnClosed called...... " )   );
    });

    _mainSocket->OnMessage().AddLambda([](const FString & Message) -> void {
        // This code will run when we receive a string message from the server.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Green, Message   );
    });

    _mainSocket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) -> void {
        // This code will run when we receive a raw (binary) message from the server.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Blue, TEXT("Binary data Arrived.... " )   );
    });

    _mainSocket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {
        // This code is called after we sent a message to the server.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Yellow, TEXT("OnMessageSent...... " )   );
    });




    _mainSocket.Get()->Connect();

     GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Red, TEXT("WebSocket Connecting....")  );


    return 0;
}

void USocketIOManager::DisconnectedFromServer()
{
    if (_mainSocket  ) {
        if( GEngine ){
            GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Red, TEXT("Websocket Close...."));
        }
         _mainSocket.Get()->Close();
    }
}

void USocketIOManager::SendTxtMsg(const FString &Msg)
{
    ensureAlwaysMsgf(_mainSocket.IsValid(),TEXT("WebSocket invalid...."));

    _mainSocket.Get()->Send(*Msg);

    if( GEngine ){
        GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Cyan, TEXT("Msg sent...."));
    }
}

void USocketIOManager::SendBinData(const TArray<uint8> &BinData)
{

}





