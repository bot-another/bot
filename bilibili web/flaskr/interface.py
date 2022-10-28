from flask import Blueprint
from flask import g
from flask import request
from flask import Flask
from flask import url_for
from flask import render_template
from werkzeug.security import check_password_hash
from werkzeug.security import generate_password_hash
import BiliUtil
import threading
import os
import time
import json
import datetime
from flaskr.db import get_db

clarity = [BiliUtil.Config.Quality.V4K, BiliUtil.Config.Quality.V1080P60, BiliUtil.Config.Quality.V1080Px,
           BiliUtil.Config.Quality.V1080P, BiliUtil.Config.Quality.V720P60, BiliUtil.Config.Quality.V720P,
           BiliUtil.Config.Quality.V480P]
bp = Blueprint("interface", __name__)
bp.send_file_max_age_default = datetime.timedelta(seconds=2)


def download(url, i=3):
    album = BiliUtil.Album()
    album.set_by_url(url)
    album_info = album.sync(cookie='SESSDATA=xxxxxxxxxxxxxxxxxxxxxx')
    video_list = album.get_video_list(cookie='SESSDATA=xxxxxxxxxxxxxxxxxxxxxx')
    path = os.path.dirname(__file__)
    for video in video_list:
        # video_in = video.sync(cookie='SESSDATA=xxxxxxxxxxxxxxxxxxxx', quality=clarity[i])
        video_name = video.video_name(BiliUtil.Config.SET_AS_NAME)
        task = BiliUtil.Task(video, path + '/static/video', str(video_name))
        task.start()
        break
    return str(video_name)


def delete(url, seconds=7200):
    time.sleep(seconds)
    if os.path.exists(url):
        os.remove(url)


@bp.route("/index", methods=("GET", "POST"))
def index():
    # print(request.args)
    # data = request.json.get()
    # print(str(data))
    url = request.args['url']
    clarity = request.args['clarity']
    videoname = download(url, int(clarity))
    path = os.path.dirname(__file__)
    res = {'url': '********************' + str(url_for('static', filename='video/' + videoname + '.mp4'))}  # *********
    DEL = threading.Thread(target=delete, args=(path + '/static/video' + videoname + '.mp4',))
    DEL.start()

    return res


@bp.route('/login', methods=('get', 'POST'))
def login():
    res = {'success': '0', 'error': ''}
    if request.method == 'POST':
        get_data = json.loads(request.get_data(as_text=True))
        # get_data = request.get_data(as_text=True)
        # print(eval(get_data))
        username = get_data.get("username")
        password = get_data.get("password")
        a = get_db()
        user = a.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            res['error'] = 'Incorrect username'  # Incorrect username.
        elif not check_password_hash(user['password'], password):
            res['error'] = 'Incorrect password'  # Incorrect password.

        if res['error'] == '':
            res['success'] = '1'  # success

    return res


@bp.route('/register', methods=('GET', 'POST'))
def register():
    res = {'success': '0', 'error': ''}
    if request.method == 'POST':
        get_data = json.loads(request.get_data(as_text=True))
        username = get_data.get("username")
        password = get_data.get("password")
        a = get_db()

        if not username:
            res['error'] = 'Username is required'
            return res  # Username is required.
        elif not password:
            res['error'] = 'Password is required'
            return res  # Password is required.

        if res['error'] == '':
            try:
                a.execute(
                    "INSERT INTO user (username, password) VALUES (?, ?)",
                    (username, generate_password_hash(password)),
                )
                a.commit()
                res['success'] = '1'
                return res  # success
            except a.IntegrityError:
                res['error'] = 'User is already registered'
                return res  # f"User {username} is already registered.
            else:
                return res
    return res


@bp.route('/message', methods=('GET', 'POST'))
def get_message():
    res = {'success': '0', 'error': '', 'message': '', 'amessage': ''}
    if request.method == 'POST':
        get_data = json.loads(request.get_data(as_text=True))
        username = get_data.get("username")
        password = get_data.get("password")
        a = get_db()
        user = a.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            res['error'] = 'Incorrect username'  # Incorrect username.
        elif not check_password_hash(user['password'], password):
            res['error'] = 'Incorrect password'  # Incorrect password.

        if res['error'] == '':
            res['success'] = '1'  # success
            res['message'] = user['message']
            res['amessage'] = user['amessage']
    return res


@bp.route('/update', methods=('GET', 'POST'))
def update_message():
    res = {'success': '0', 'error': ''}
    if request.method == 'POST':
        get_data = json.loads(request.get_data(as_text=True))
        username = get_data.get("username")
        password = get_data.get("password")
        message = get_data.get("message")
        amessage = get_data.get("amessage")
        db = get_db()
        user = db.execute(
            'SELECT * FROM user WHERE username = ?', (username,)
        ).fetchone()

        if user is None:
            res['error'] = 'Incorrect username'  # Incorrect username.
        elif not check_password_hash(user['password'], password):
            res['error'] = 'Incorrect password'  # Incorrect password.

        if res['error'] == '':
            db.execute(
                'UPDATE user SET message = ?, amessage = ?',
                (message, amessage)
            )
            db.commit()
            res['success'] = '1'  # success
    return res


@bp.route('/main', methods=('GET', 'POST'))
def main():
    return render_template('index.html')
