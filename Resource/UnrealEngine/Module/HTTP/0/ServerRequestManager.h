
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "ServerRequestManager.generated.h"


DECLARE_DELEGATE_FourParams(FOnRecieveResponse_Custom, FString /* path*/, bool /*success*/, TSharedPtr<FJsonObject>/*data*/, TSharedPtr<FJsonObject>/*err*/)

/**
 * 
 */
UCLASS()
class MV_FAIRYTAIL_API UServerRequestManager : public UObject
{
	GENERATED_BODY()


	// http 모듈
	FHttpModule* HttpModule;
	
	UPROPERTY()
	FString serverURL;	// 서버주소

	/* 서버에서 응답이 왔을때 널리 알리는 호출 대행자 */
	FOnRecieveResponse_Custom OnServerResponse;

public:

	UServerRequestManager();

public:

	/* 서버의 주소를 설정합니다.
	* @Param newServerURI 새로운 서버의 주소
	*/
	UFUNCTION(BlueprintCallable)
	void setServerURL(const FString newServerURI);

	/* 서버의 주소를 반환합니다.
	* @Return server 의 주소
	*/
	UFUNCTION(BlueprintCallable)
	FString getServerURL() const;

	/* 기본적인 http Request 를 만들어 반환합니다.
	* @Param Method 어떤 요청으로 보낼것인지 적어줍니다.
	* @Param path 경로를 적어줍니다.
	* @Param ContentBody http 요청에 실어보낼 내용을 적어줍니다.
	* @Param token 토큰을 사용한다면 넣어주자.
	*/
	TSharedRef<IHttpRequest> makeDefaultHTTPRequest (const FString method, const FString path, const FString contentBody, const FString token = TEXT(""));

	/* 서버에서 응답이 도착했을때 호출되는 델리게이트를 반환합니다.*/
	FOnRecieveResponse_Custom* getDelegate_Response();

private:
	/* HTTP 응답을 받을때 바인딩할 함수*/
	void OnResponseReceivedRaw(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	

};