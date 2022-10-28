{
    var username
    var password
    var loginStatus = false
    function register() {
        let username = $("#username").val()
        let password = $("#password").val()
        console.log(username, password)
        if (username == "") {
            alert("用户名为空")
            return 0
        }
        if (password == "") {
            alert("密码为空")
            return 0
        }
        $.ajax({
            url: "http://119.3.128.207:5000/register",
            type: "POST",
            dataType: "json",
            content: "application/json;",
            data: JSON.stringify({
                username: username,
                password: password
            }),
            beforeSend: function () {

            },
            success: function (res) {
                console.log(res)
                if (res.success != 1) {
                    if (res.error == "User is already registered") {
                        alert("用户已存在")
                    }
                }
                else {
                    alert("注册成功")
                    if(confirm("是否下次自动登录？") == true){
                        setCookie("username", username, 30)
                        setCookie("password", password, 30)
                        update()
                    }
                    
                }

            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                console.log(XMLHttpRequest.status)
                console.log(XMLHttpRequest.readyState)
                console.log(textStatus)
            },
            complete: function () {

            }
        })
    }
    function login() {
        let username = $("#username").val()
        let password = $("#password").val()
        if (username == "") {
            alert("用户名为空")
            return 0
        }
        $.ajax({
            url: "http://119.3.128.207:5000/login",
            type: "post",
            dataType: "json",
            contentType: "application/json",
            data: JSON.stringify({
                username: username,
                password: password
            }),
            beforeSend: function () {

            },
            success: function (res) {
                setCookie("username", username, 30)
                setCookie("password", password, 30)
                console.log(res)
                if (res.success == "1") {
                    loginStatus = true
                    alert("成功登录")
                    $("#toolDiv2").css({
                        display: "none"
                    })

                    $("#previewDiv2").css({
                        display: "none"
                    })
                    $("#status").css({
                        display: "block"
                    })
                }
                else {
                    alert("登录失败")
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                console.log(XMLHttpRequest.status)
                console.log(XMLHttpRequest.readyState)
                console.log(textStatus)
            },
            complete: function () {

            }
        })
    }
    function update() {
        let message = $("#timeFlow").prop("outerHTML")
        let amessage = $("#gridCard").prop("outerHTML")
        console.log(username, password)
        $.ajax({
            url: "http://119.3.128.207:5000/update",
            type: "POST",
            dataType: "json",
            contentType: "application/json",

            data: JSON.stringify({
                username: username,
                password: password,
                message: message,
                amessage: amessage
            }),
            beforeSend: function () {
            },
            success: function (res) {
                if (loginStatus == false) {
                    alert("上传到云端需要登录，请登录")
                    return 0
                }
                console.log(res)
                if (res.success == 1) {
                    alert("上传成功")
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                console.log(XMLHttpRequest.status)
                console.log(XMLHttpRequest.readyState)
                console.log(textStatus)
            },
            complete: function () {
            }
        })
    }
    function download() {
        $.ajax({
            url: "http://119.3.128.207:5000/message",
            type: "POST",
            dataType: "json",
            contentType: "application/json",
            data: JSON.stringify({
                username: username,
                password: password
            }),
            beforeSend: function () {

            },
            success: function (res) {
                if (loginStatus == false) {
                    alert("登录后同步数据")
                    return 0
                }
                console.log(res)
                if (confirm("确定要同步云端信息吗？") == true) {
                    $("#timeFlow").remove()
                    $("#gridCard").remove()
                    $("#longCard").append(res.message)
                    $("#con").append(res.amessage)
                }
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                console.log(XMLHttpRequest.status)
                console.log(XMLHttpRequest.readyState)
                console.log(textStatus)
            },
            complete: function () {

            }
        })
    }
    function WENcode(password) {
        for (let i = 0; i < password.length; ++i) {
            password[i] = password[i] ^ 0x25
        }
        return password
    }
    function setCookie(key, value, exdays) {
        var d = new Date()
        d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000))
        var expires = "expires=" + d.toGMTString()
        document.cookie = key + "=" + value + "; " + expires
        console.log(document.cookie)
    }
    function checkCookie() {
        var username = getCookie("username")
        var password = getCookie("password")
        if (username != "") {
            $("#username").val(username)
            $("#password").val(password)
        }
    }
}

window.onload = function () {
    function getCookie(param) {
        var patt = new RegExp(param + "=" + "([0-9a-zA-Z]*)" + "(;)?")
        var cookies = document.cookie
        if (cookies == "") return 0
        var raw = cookies.match(patt)
        return raw[1]
    }
    username = getCookie("username")
    password = getCookie("password")
    $('#password').focus()
    $("#password").blur()
    console.log("______________________________________________")
    if (username != "") {
        $("#username").val(username)
        $("#password").val(password)
        setTimeout(login(), 2000)
    }
}
// 119.3.128.207:5000/login
// 发送json.dumps{'user':'xxxx','password':xxxx}
// 119.3.128.207:5000/register
// 发送json.dumps{'user':'xxxx','password':xxxx}
// 119.3.128.207:5000/message（得到页面数据）
// 发送json.dumps{'user':'xxxx','password':xxxx}
// 119.3.128.207:5000/update（就是这个，undate，你测试好了再改，修改页面数据）
// 发送json.dumps{'user':'xxxx','password':xxxx,'message':'xxxx'}