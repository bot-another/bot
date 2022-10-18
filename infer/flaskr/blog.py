import numpy as np
import paddle
from PIL import Image
from flask import redirect, Blueprint
from flask import render_template
from flask import request
from flask import url_for

from flask import redirect, make_response,jsonify
from werkzeug.utils import secure_filename
import os
import cv2
import time
import datetime
# 预处理图片
from paddle import fluid
import threading


def load_image(file):
    img = Image.open(file)
    # 统一图像大小
    img = img.resize((224, 224), Image.ANTIALIAS)
    # 转换成numpy值
    img = np.array(img).astype(np.float32)
    # 转换成CHW
    img = img.transpose((2, 0, 1))
    # 转换成BGR
    img = img[(2, 1, 0), :, :] / 255.0
    img = np.expand_dims(img, axis=0)
    return img


def load_image(file):
    img = Image.open(file)
    # 统一图像大小
    img = img.resize((224, 224), Image.ANTIALIAS)
    # 转换成numpy值
    img = np.array(img).astype(np.float32)
    # 转换成CHW
    img = img.transpose((2, 0, 1))
    # 转换成BGR
    img = img[(2, 1, 0), :, :] / 255.0
    img = np.expand_dims(img, axis=0)
    return img


def infer(img_path):
    # 创建执行器
    place = fluid.CPUPlace()
    exe = fluid.executor.Executor(place)
    paddle.enable_static()
    # exe.run(fluid.default_startup_program())

    # 保存预测模型路径
    save_path = '/home/infer/flaskr/infer_model/'
    # 从模型中获取预测程序、输入数据名称列表、分类器
    [infer_program, feeded_var_names, target_var] = fluid.io.load_inference_model(dirname=save_path, executor=exe)
    # 获取图片数据
    img = load_image(img_path)

    # 执行预测
    result = exe.run(program=infer_program,
                     feed={feeded_var_names[0]: img},
                     fetch_list=target_var)
    # 显示图片并输出结果最大的label
    lab1 = np.argsort(result)[0][0][-1]
    lab2 = np.argsort(result)[0][0][-2]
    names = ['梨子', '樱桃', '哈密瓜', '胡萝卜', '黄瓜', '西瓜']

    result1 = ('名称为：%s， 概率为：%f ' % (names[lab1], result[0][0][lab1]))
    result2 = ('名称为：%s， 概率为：%f ' % (names[lab2], result[0][0][lab2]))
    return result1,result2


bp = Blueprint("blog", __name__)


ALLOWED_EXTENSIONS = set(['png', 'jpg', 'jpeg'])
 
def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS
 
# 设置静态文件缓存过期时间
bp.send_file_max_age_default = datetime.timedelta(seconds=1)
 
@bp.route("/error", methods=['POST', 'GET'])
def error():
    if request.method == "POST":
        value = request.form["value"]
        return redirect(url_for("blog.index"))
    return render_template("error.html")



@bp.route("/", methods=['POST', 'GET'])  # 添加路由
def index():
    if request.method == 'POST':
        f = request.files['file']
 
        if not (f and allowed_file(f.filename)):
            return redirect(url_for("blog.error"))
 
 
        basepath = os.path.dirname(__file__)  # 当前文件所在路径
 
        upload_path = os.path.join(basepath, 'static/images', secure_filename(f.filename))  # 注意：没有的文件夹一定要先创建，不然会提示没有该路径
        # upload_path = os.path.join(basepath, 'static/images','test.jpg')  #注意：没有的文件夹一定要先创建，不然会提示没有该路径
        f.save(upload_path)
 
        # 使用Opencv转换一下图片格式和名称
        img = cv2.imread(upload_path)
        cv2.imwrite(os.path.join(basepath, 'static/images', f.filename), img)
        (result1,result2)=infer('/home/infer/flaskr/static/images/'+f.filename)
        DEL = threading.Thread(target=delete,args = ('/home/infer/flaskr/static/images/'+f.filename,))
        DEL.start()
        return render_template('index_ok.html',val1=time.time(), img='./images/'+f.filename ,result1=result1 ,result2=result2)
    return render_template('index.html')
def delete(path):
    time.sleep(100)
    if os.path.exists(path):
        os.remove(path)