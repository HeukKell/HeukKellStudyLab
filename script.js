{
    /** HTML 파일을 텍스트로 반환하는 함수 */
    async function fetchHTMLAsText(url){
        return await (await fetch(url)).text();
    }
    
    // Navigation bar 그리기
    const NavElem = document.querySelector(".NavBar");
    fetchHTMLAsText("/Component/NavigationBarComponent.html")
    .then((result)=>{
        NavElem.innerHTML = result;
    })

    const MemoElem = document.querySelector("#MemoComp");
    fetchHTMLAsText("/Component/MemoComp.html")
    .then((result)=>{
        let newMemo=result.replace("__Content__",MemoElem.innerHTML); // MemoComp 의 내용을 MemoComp 아이디를 가진 태그의 내용으로 덮어씌워 적용
        MemoElem.innerHTML = newMemo;
    })

}