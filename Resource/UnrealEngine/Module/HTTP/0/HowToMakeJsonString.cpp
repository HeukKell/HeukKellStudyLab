
#include "dom/JsonObject.h" // FJsonObject 를 사용할 수 있게 합니다.
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


// JSON 오브젝트 만들고 값 넣기

TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
RequestObj->SetStringField("FieldName", "FieldValue");

// JSON 오브젝트를 FString 으로 직렬화 하여 담기

FString RequestBody;
TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
FJsonSerializer::Serialize(RequestObj,Writer);
