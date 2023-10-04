if(location.hash.indexOf('?')>-1)
{
    var urlp = location.hash.split('?');
    if(urlp.length>0)
    {
        var up = urlp[1].split('&');
        for(var u in up)
        {
            var surlp = up[u].split('=');
            for(var s in surlp)
            {
                if(surlp[0] === 'id')
                {
                    // console.log(parseInt(window.pageYOffset), window.document.getElementById(decodeURI(surlp[1])).offsetTop)
                    // console.log(parseInt(window.document.body.scrollHeight)-parseInt(window.document.body.clientHeight))

                    if(parseInt(window.pageYOffset)!==0 && parseInt(window.pageYOffset) < window.document.getElementById(decodeURI(surlp[1])).offsetTop)
                    {
                        var setT = setTimeout(function(){
                            location.reload();
                            if(parseInt(window.pageYOffset) >= window.document.getElementById(decodeURI(surlp[1])).offsetTop)
                                clearTimeout(setT);
                        },500);
                        //     console.log(parseInt(window.pageYOffset) < window.document.getElementById(decodeURI(surlp[1])).offsetTop)

                        console.log(parseInt(window.document.body.scrollHeight)-parseInt(window.document.body.clientHeight),parseInt(window.document.getElementById(decodeURI(surlp[1])).offsetTop))
                        //页面全高-屏幕高度如果大于滚动栏高度,则清除滚动
                        if(parseInt(window.document.body.scrollHeight)-parseInt(window.document.body.clientHeight)<parseInt(window.document.getElementById(decodeURI(surlp[1])).offsetTop))
                        {
                            clearTimeout(setT);
                        }
                    }

                }
            }
        }
    }
}