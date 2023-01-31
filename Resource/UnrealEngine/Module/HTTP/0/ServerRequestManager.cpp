
// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRequestManager.h"


UServerRequestManager::UServerRequestManager()
{
	HttpModule = &FHttpModule::Get();
}

inline void UServerRequestManager::setServerURL(const FString newServerURI)
{
	serverURL = newServerURI;
}

inline FString UServerRequestManager::getServerURL() const
{
	return serverURL;
}

TSharedRef<IHttpRequest> UServerRequestManager::makeDefaultHTTPRequest(const FString method, const FString path,const FString contentBody, const FString token)
{
	if (nullptr == HttpModule){
		HttpModule = &FHttpModule::Get();
	}

	// Rquest 하나 만들자
	TSharedRef<IHttpRequest> Request = HttpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UServerRequestManager::OnResponseReceivedRaw);	// Multicast Delegate. 함수 바인딩

	// Request 에 쓰기
	Request->SetURL(*FString::Printf(TEXT("%s%s"),*serverURL, *path));
	
	Request->SetVerb(method);

	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	//Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));

	if (method.Equals(TEXT("GET"))){
		UE_LOG(LogTemp, Warning, TEXT("현재 GET 요청에 ContentBody 를 실으면 문제가 있어 넣지 않았습니다."));
	}
	else {
		Request->SetContentAsString(contentBody);
	}

	if (TEXT("") != token){	// 토큰이 있으면 추가
		Request->SetHeader(TEXT("authorization"), token);
	}

	UE_LOG(LogTemp, Warning, TEXT("makeHttpRequest\n %s(%s%s)\n token=%s,\ncontentBody=%s }"), *method, *serverURL, *path, TEXT("")==token ? TEXT("") : *token, *contentBody);
	return Request;
}


void UServerRequestManager::OnResponseReceivedRaw(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	
	FString URL = Request->GetURL();
	FString path;
	
	for (int32 i = serverURL.Len(); i < URL.Len(); i++){
		path = path + URL[i];	// path 만 목사
	}

	// 내용
	TSharedPtr<FJsonObject> ResponseObj;	// Json 내용을 받을 JsonObject
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString()); // 서버로 응답받은 내용을 Json 으로 읽는 Json Reader 생성

	if (FJsonSerializer::Deserialize(Reader, ResponseObj)){

		// 성공시 데이터라는 값이 나올꺼야.
		if (ResponseObj->HasField(TEXT("data")))
		{
			// data 라는 필드가 있다면
			TSharedPtr<FJsonObject> data = ResponseObj->GetObjectField(TEXT("data"));
			
			OnServerResponse.Execute(path, true, data, nullptr);
		}
		else {
			// err 필드가 있다면
			TSharedPtr<FJsonObject> err = ResponseObj->GetObjectField(TEXT("err"));
			OnServerResponse.Execute(path, false, nullptr, err);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("failed convert Response object!"));
	}

	//OnServerResponse.Broadcast(path,)
}

inline FOnRecieveResponse_Custom* UServerRequestManager::getDelegate_Response()
{
	return &OnServerResponse;
}