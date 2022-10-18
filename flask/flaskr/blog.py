from flask import Blueprint
from flask import flash
from flask import g
from flask import redirect
from flask import render_template
from flask import request
from flask import url_for
from flask import Flask, send_from_directory
from werkzeug.exceptions import abort
import re
import os
import time
import sys
import re
from flask import make_response
from flask import after_this_request

t_dict = {}
import BiliUtil
clarity=[BiliUtil.Config.Quality.V4K, BiliUtil.Config.Quality.V1080P60, BiliUtil.Config.Quality.V1080Px, BiliUtil.Config.Quality.V1080P, BiliUtil.Config.Quality.V720P60, BiliUtil.Config.Quality.V720P, BiliUtil.Config.Quality.V480P]
def download(url,i = 3):
    if re.findall('^https://www\.bilibili\.com/\w+/\w+/?\w+', url, re.S):
        album = BiliUtil.Album()
        album.set_by_url(url)
        album_info = album.sync(cookie='SESSDATA=xxxx')
        video_list = album.get_video_list(cookie='SESSDATA=xxxx')
        for video in video_list:
            video_in = video.sync(cookie='SESSDATA=xxxx',quality=clarity[i])
            video_name = video.video_name(BiliUtil.Config.SET_AS_NAME)
            t_dict[url+"name"] = video_name
            task = BiliUtil.Task(video, '/home/flask/video', video_name)
            task.start()      
    else:
        t_dict[url+"name"] = "error"

def delete(url,seconds=7200):
    time.sleep(seconds)
    if os.path.exists(url):
        os.remove(url)
    del t_dict[url+'delete']
import threading
bp = Blueprint("blog", __name__)
@bp.route("/Download/<videoname>", methods=("GET", "POST"))
def Download(videoname):
    response = make_response(send_from_directory(path=('/home/flask/video/' + videoname + '.mp4'),directory="/home/flask/video",filename=(videoname + '.mp4'),as_attachment=True))
    return response
    # video_path = '/home/flask/video/' + videoname
    # subprocess.call('rm -rf '+ video_path)
@bp.route("/error", methods=("GET", "POST"))
def error():
    if request.method == "POST":
        value = request.form["value"]
        return redirect(url_for("blog.index"))
    return render_template("error.html")
@bp.route("/success/<videoname>", methods=("GET", "POST"))
def success(videoname):
    a = videoname
    url = '/home/flask/video/' + videoname + '.mp4'
    if request.method == "POST":
        value = request.form["value"]
        if value == '下载':
            if url+'delete' not in t_dict:
                DEL = threading.Thread(target=delete,args = (url,))
                DEL.start()
                t_dict[url+'delete']=DEL
            return redirect(url_for("blog.Download", videoname = a))
        else:
            if url+'delete' not in t_dict:
                DEL = threading.Thread(target=delete,args = (url,))
                DEL.start()
                t_dict[url+'delete']=DEL
            return redirect(url_for("blog.index"))
    return render_template("success.html")
@bp.route("/wait/<videoname>", methods=("GET", "POST"))
def wait(videoname):
    a = videoname
    if t_dict[a].is_alive() == False: 
        del t_dict[a]
        return redirect(url_for("blog.success", videoname = a))
    if request.method == "POST":
        return redirect(url_for("blog.wait", videoname = a))
    return render_template("wait.html")
@bp.route("/", methods=("GET", "POST"))
def index():
    if request.method == "POST":
        url = request.form['url']
        options = request.form.get('clarity')
        print(options)
        t = threading.Thread(target=download,args = (url,int(options),))
        t.start()
        while url+"name" not in t_dict:
            continue
        videoname=t_dict[url+"name"]
        del t_dict[url+"name"]
        t_dict[videoname] = t
        if videoname=="error":
            return redirect(url_for("blog.error"))
        else:
            return redirect(url_for("blog.wait", videoname = videoname))
    return render_template("index.html")
# from DecryptLogin import login
# _, session = login.Login().bilibili(username, password)

