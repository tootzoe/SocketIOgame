// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

//#include "WebSocketsModule.h"

#include "SocketIOManager.generated.h"


class IWebSocket;
class IWebSocketsManager;

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


 private:


    TSharedPtr <IWebSocket> _mainSocket;

};
