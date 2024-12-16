// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

//#include "WebSocketsModule.h"

#include "SocketIOManager.generated.h"


class IWebSocket;
class IWebSocketsManager;




#if 1
UENUM(BlueprintType)
enum  EWebSocketStatus : uint8
{
    E_Disconnected UMETA(DisplayName="Disconnected"),
    E_Connecting UMETA(DisplayName="Connecting"),
    E_Connected UMETA(DisplayName="Connected"),
    E_ConnectionError UMETA(DisplayName="Connection Error"),
    E_ServerClosed UMETA(DisplayName="Server Closed"),
    E_TextMsg UMETA(DisplayName="Text Msg"),
    E_TextMsgSent UMETA(DisplayName="Text Msg Sent"),
    E_BinaryData UMETA(DisplayName="Binary Data"),
    //
    E_Max UMETA(Hidden)
};
#endif


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWssStatusSignature,  EWebSocketStatus  , WssStatus, const TArray<uint8>& ,  BinDataA);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SOCKETIOGAME_API USocketIOManager : public UObject
{
	GENERATED_BODY()
	
  public:

    explicit USocketIOManager();
     ~USocketIOManager();
	

    UFUNCTION(BlueprintCallable, Category = "TOOT")
    void InitializeSocketIO();
    UFUNCTION(BlueprintCallable, Category = "TOOT")
    int32 ConnectToUrl(const FString& Url);
    UFUNCTION(BlueprintCallable, Category = "TOOT")
    void DisconnectedFromServer( );
    //
    UFUNCTION(BlueprintCallable, Category = "TOOT")
    void SendTxtMsg( const FString& Msg );
    UFUNCTION(BlueprintCallable, Category = "TOOT")
    void SendBinData( const TArray<uint8>& BinData );
    //
    UFUNCTION(BlueprintImplementableEvent, Category = "TOOT")
    void WssStatusEvent( EWebSocketStatus EStatus,  const TArray<uint8>& BinData );
    //
    UFUNCTION(BlueprintCallable, Category = "TOOT")
    void Uint8ArrToString( const TArray<uint8>& BinData , FString &RslStr );

    // UPROPERTY(BlueprintAssignable, Category="TOOT")
    // FWssStatusSignature OnWssStatusChged;

   // UFUNCTION()

 private:

    static FWssStatusSignature OnWssStatusChged;
    TSharedPtr <IWebSocket> _mainSocket;

};
