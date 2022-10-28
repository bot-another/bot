function darkMode() {
    $("html").css({
        filter: "invert(1) hue-rotate(200deg)"
    })
    $("#tTL02").css({
        display: "none"
    })
    $("#tTL01").css({
        display: "block"
    })
}
function normalMode() {
    $("html").attr({
        style: ''
    })
    $("#tTL02").css({
        display: "block"
    })
    $("#tTL01").css({
        display: "none"
    })
}
function Grid() {
    $("#tTL03").css({
        display: "none"
    })
    $("#tTL04").css({
        display: "block"
    })
    $("#longCard").animate({
        width: "0%"
    },
        500,
        function () {
            $("#longCard").css({
                display: "none"
            })
            $("#gridCard").css({
                display: "flex"
            })
            $("#gridCard").animate({
                width: "72%"
            },
                500)
        })
}
function List() {
    $("#tTL03").css({
        display: "block"
    })
    $("#tTL04").css({
        display: "none"
    })
    $("#longCard").css({
        display: "block"
    })
    $("#longCard").animate({
        width: "72%"
    },
        1000,
    )
    $("#gridCard").animate({
        width: "0%"
    },
        500,
        function () {
            $("#gridCard").css({
                display: "none"
            })
            $("#longCard").css({
                display: "flex"
            })
            $("#longCard").animate({
                width: "72%"
            },
                500)
        })
}
function HiddenDiv() {
    $("#integrateDiv").animate({
        left: '98%'
    })
    $("#hiddenButton1").css({
        display: "none"
    })
    $("#hiddenButton2").css({
        display: "block"
    })
}
function ShowDiv() {
    $("#integrateDiv").animate({
        left: '74%'
    })
    $("#hiddenButton2").css({
        display: "none"
    })
    $("#hiddenButton1").css({
        display: "block"
    })
}
function writeInput() {
    $("#clarity").val($(event.currentTarget).text())
}


{
    function setId(item) {
        var id = item.id
        let pos = Number(id.search(/\d+$/))
        let num = String(Number(id.substring(pos)) + 1)
        id = id.replace(/\d+$/, num)
        item.id = id
        return num
    }
    function setAll(node) {
        var nodeList = node.childNodes
        for (var i = 0; i < nodeList.length; i++) {
            var childNode = nodeList[i]
            if (childNode.nodeType == 1) {
                setId(childNode)
                setAll(childNode)
            }
        }
    }
    function cheakStr(str1, str2) {
        let pattern1 = new RegExp(/(https:\/\/)?(www.bilibili.com\/video\/)(BV|AV)[0-9A-Za-z]{10}/)
        let pattern2 = new RegExp(/(BV|AV)[0-9A-Za-z]{10}/)
        let result = pattern1.test(str1) || pattern2.test(str2)
        if (!result) {
            alert("请输入正确的视频网址")
        }
        return result
    }
    function setInfo(num, code) {
        switch (code) {
            case 1:
                {
                    $("#4p" + num).text($("#hrefInput").val())
                    $("#5p" + num).text($("#clarity").val())
                    var d = new Date();
                    var y = d.getFullYear()
                    var m = d.getMonth() + 1
                    var date = d.getDate()
                    var h = d.getHours()
                    var min = d.getMinutes()
                    var time = y + "年" + m + "月" + date + "日" + h + ": " + min
                    $("#timep" + num).text(time)
                }
                break;
            case 2:
                {
                    console.log($("#hrefInput").val())
                    $("#10p" + num).text($("#hrefInput").val())
                    $("#hrefInput").val("")
                    $("#11p" + num).text($("#clarity").val())
                    $("#clarity").val("")
                }
                break;
        }
    }
    function addTimeFlow() {
        if (cheakStr($("#hrefInput").val(), $("clarity").val())) {
            ajaxGet()
            $("#timeFlow").prepend($("#newNode0").clone(true, true))
            var item = document.getElementById("timeFlow").firstChild
            let num = setId(item)
            setAll(item)
            setInfo(num, 1)
            $("#gridCard").prepend($("#oS0").clone(true, true))
            var obj = document.getElementById("gridCard").firstChild
            let one = setId(obj)
            setAll(obj)
            setInfo(one, 2)
        }
    }
    var dict = {
        "720p&60FPS": 4,
        "标清480p": 6,
        "高清720p": 5,
        "超清1080p": 3,
        "10800p&60FPS": 1,
        "蓝光4K": 0,
    }

    function ajaxGet() {
        $.ajax({
            type: "GET",
            url: "http://119.3.128.207:5000/index",
            timeout: 300000,
            data: {
                url: $("#hrefInput").val(),
                clarity: dict[$("#clarity").val()]
            },
            async: true,
            beforeSend: function () {
                $("#button").css({
                    display: "none"
                })
            },
            success: function (res) {
                console.log(res.url)
                downLoadFile(res.url)
                // http://119.3.128.207:2000/static/video/%E4%BA%8C%E8%83%A1%E7%89%88___%E5%8D%83%E6%9C%AC%E6%A8%B1_P1_%E9%AB%98%E6%B8%85%201080P+.mp4
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                alert(XMLHttpRequest.status);
                alert(XMLHttpRequest.readyState);
                alert(textStatus);
            },
            complete: function () {
                $("#button").css({
                    display: "block"
                })
            }
        });
    }
    function downLoadFile(url) {
        fetch(url).then(res => res.blob()).then(blob => {
            let Unicode = Date.now()
            const Url = window.URL.createObjectURL(blob)
            var ele = $("<a>nihao</a>").attr({
                "id": Unicode,
                "href": Url,
                "download": "video" + Unicode + ".mp4"
            })
            ele.css({
                position: "relative",
                width: "30px",
                height: "30px"
            })
            $("body").append(ele)
            document.getElementById(Unicode).click()
            $("#" + Unicode).remove()
            window.URL.revokeObjectURL(Url);
        })
    }
}
function deleteItem() {
    let Parents = $(event.currentTarget).parents()
    let id = $(Parents[3]).attr("id")
    let num = id[Number(id.search(/\d+$/))]
    if (num == 0) return 0
    let msg = "确认删除该时间流？"
    if (confirm(msg) == true) {
        $("#newNode" + num).remove()
        $("#oS" + num).remove()
        return true
    } else {
        return false
    }
}

// https://www.bilibili.com/video/BV1XF411F7Fw?spm_id_from=333.1007.tianma.3-1-5.click&vd_source=613b095f1e656b9e7b1c2e81131292ea
// https://www.bilibili.com/video/BV1eU4y1X7EB?spm_id_from=333.1073.high_energy.content.click&vd_source=613b095f1e656b9e7b1c2e81131292ea
// https://www.bilibili.com/video/BV1jt4y1h7Wo?spm_id_from=333.1007.tianma.2-1-3.click&vd_source=613b095f1e656b9e7b1c2e81131292ea
// clarity = [BiliUtil.Config.Quality.V4K, BiliUtil.Config.Quality.V1080P60, BiliUtil.Config.Quality.V1080Px,
// BiliUtil.Config.Quality.V1080P, BiliUtil.Config.Quality.V720P60, BiliUtil.Config.Quality.V720P,
// BiliUtil.Config.Quality.V480P]
// http://119.3.128.207:2000/static/video/%E4%BA%8C%E8%83%A1%E7%89%88___%E5%8D%83%E6%9C%AC%E6%A8%B1_P1_%E6%B8%85%E6%99%B0%20480P.mp4