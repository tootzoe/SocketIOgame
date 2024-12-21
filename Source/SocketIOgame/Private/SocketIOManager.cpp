// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketIOgame/Public/SocketIOManager.h"

#include <string>
#include <vector>

#include "WebSocketsModule.h"
#include "IWebSocket.h"



 FWssStatusSignature USocketIOManager::OnWssStatusChged  ;

USocketIOManager::USocketIOManager() :
    _mainSocket(nullptr)
{
    OnWssStatusChged.Clear();
    OnWssStatusChged.AddUniqueDynamic(this, &USocketIOManager::WssStatusEvent);

}

USocketIOManager::~USocketIOManager()
{
    //UE_LOG(LogTemp, Warning, TEXT("USocketIOManager deconstructed............"));
   // DisconnectedFromServer();

}


void USocketIOManager::InitializeSocketIO()
{
    if(!   FModuleManager::Get().IsModuleLoaded("WebSockets") ){
        FModuleManager::Get().LoadModule("WebSockets");
    }

}


static void FStringToUin8Arr(const FString& Message, TArray<uint8>& binDataArr )
{
    int32 Utf8Length = FTCHARToUTF8_Convert::ConvertedLength(*Message, Message.Len());
    binDataArr.SetNumUninitialized(Utf8Length);
    FTCHARToUTF8_Convert::Convert((UTF8CHAR*)binDataArr.GetData(), binDataArr.Num(), *Message, Message.Len());
}

int32 USocketIOManager::ConnectToUrl(const FString &Url)
{
    _mainSocket =  FWebSocketsModule::Get().CreateWebSocket(*Url, TEXT("wss") ); // "ws://127.0.0.1:8080"

    ensureAlwaysMsgf(_mainSocket.IsValid() , TEXT("Create WebSocket Failed....")  );

    //Setup listener.......
    _mainSocket->OnConnected().AddLambda([]() -> void {
        // This code will run once connected.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Green, TEXT("OnConnected ok.... " )   );
          UE_LOG(LogTemp, Warning, TEXT("OnConnected() called........."));
           TArray<uint8> binDataArr;
          OnWssStatusChged.Broadcast( E_Connected , binDataArr);
    });

    _mainSocket->OnConnectionError().AddLambda([](const FString & Error) -> void {
        // This code will run if the connection failed. Check Error to see what happened.
         GEngine->AddOnScreenDebugMessage(-1, 16.f, FColor::Red, Error  );
         UE_LOG(LogTemp, Warning, TEXT("OnConnectionError().... Err: %s"), *Error);

          TArray<uint8> binDataArr;
          FStringToUin8Arr(Error , binDataArr);
          OnWssStatusChged.Broadcast( E_ConnectionError ,  binDataArr );
    });

    _mainSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
        // This code will run when the connection to the server has been terminated.
        // Because of an error or a call to Socket->Close().
         UE_LOG(LogTemp, Warning, TEXT("OnClosed() called....") );
         TArray<uint8> binDataArr;
        OnWssStatusChged.Broadcast( E_ServerClosed, binDataArr);
    });

    _mainSocket->OnMessage().AddLambda([](const FString & Message) -> void {
        // This code will run when we receive a string message from the server.
        // UE_LOG(LogTemp, Warning, TEXT("TxtData: len= %ld , %s"), Message.Len()  , *Message);

         TArray<uint8> binDataArr;
         FStringToUin8Arr(Message , binDataArr);
         OnWssStatusChged.Broadcast( E_TextMsg ,  binDataArr );
    });

    _mainSocket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) -> void {
        // This code will run when we receive a raw (binary) message from the server.
        UE_LOG(LogTemp, Warning, TEXT("--BinData: len= %ld , BytesRemaining = %ld"), Size , BytesRemaining);
       // UE_LOG(LogTemp, Warning, TEXT("BinData: %hs "), (char*)Data);

        TArray<uint8> binDataArr;
        binDataArr.Append((const unsigned char*)Data, Size);

         // const FString str =  UTF8_TO_TCHAR(binDataArr.GetData());
       // UE_LOG(LogTemp, Warning, TEXT("BinData: %s "), *str);

         OnWssStatusChged.Broadcast( E_BinaryData ,  binDataArr );

    });

    _mainSocket->OnMessageSent().AddLambda([](const FString& Message) -> void {
        // This code is called after we sent a message(*ONLY Text* msg) to the server.
        // UE_LOG(LogTemp, Warning, TEXT("OnMessageSent() called.... msg: %s"), *Message);

         TArray<uint8> binDataArr;
           FStringToUin8Arr(Message , binDataArr);
         OnWssStatusChged.Broadcast( E_TextMsgSent ,  binDataArr );

    });


    _mainSocket.Get()->Connect();

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
}

void USocketIOManager::SendBinData(const TArray<uint8> &BinData)
{
    ensureAlwaysMsgf(_mainSocket.IsValid(),TEXT("WebSocket invalid...."));

    _mainSocket.Get()->Send(BinData.GetData(), sizeof(uint8) * BinData.Num(), true);
}

void USocketIOManager::Uint8ArrToString(const TArray<uint8> &BinData , FString & RslStr)
{
     RslStr = FString( BinData.Num(), UTF8_TO_TCHAR(BinData.GetData()));
}





