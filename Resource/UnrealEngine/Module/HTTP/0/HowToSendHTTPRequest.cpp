
#include "Http.h"
#include "dom/JsonObject.h" // FJsonObject 를 사용할 수 있게 합니다.
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


// HTTP 요청을 만들고 보내는 방법
void ServerRequsetManager::MakeHTTPRequsetAndProcess()
{

    // JSON 오브젝트 만들고 값 넣기
    TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
    RequestObj->SetStringField("FieldName", "FieldValue");

    // JSON 오브젝트를 FString 으로 직렬화 하여 담기
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestObj,Writer);


    // HTTP 요청 만들기
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    // HTTP 요청 보내기
    Request->OnProcessRequestComplete().BindUObject(this,&ServerRequsetManager::OnResponseReceived); // 응답함수 바인드
    Request->SetURL("https://serverurl.com/path"); // 서버 경로 설정
    Request->SetVerb("POST"); // 메소드
    Request->SetHeader("Content-Type","application/json"); // 헤더 설정
    Request->SetContentAsString(RequestBody); // JSON 오브젝트를 직렬화한 FString 을 담아 보낸다.
    Request->ProcessRequset(); // 요청 보내기

}

// HTTP 요청을 받는 방법
void ServerRequsetManager::OnResponseReceived(FHttpRequsetPtr Requset, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    // 응답 내용을 String 으로 받을 수 있다.
    Response->GetContentAsString();

    // 응답 내용을 JSON Object 로 받기
    TSharedPtr<FJsonObject> ResponseObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    FJsonSerializer::Deserialize(Reader, ResponseObj);

    // 응답 내용의 특정 값 가져오기 확인
    FString FieldValue = ResponseObj->GetStringField("FieldKey");
}