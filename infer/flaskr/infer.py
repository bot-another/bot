import paddle.fluid as fluid
from PIL import Image
import numpy as np
import paddle
import os
import imghdr

key_words = {'西瓜': 'watermelon', '哈密瓜': 'cantaloupe',
             '樱桃': 'cherry', '梨子': 'pear', '黄瓜': 'cucumber', '胡萝卜': 'carrot'}


# 预处理图片
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
    save_path = './infer_model/'
    # 从模型中获取预测程序、输入数据名称列表、分类器
    [infer_program, feeded_var_names, target_var] = fluid.io.load_inference_model(dirname=save_path, executor=exe)
    # 获取图片数据
    img = load_image(img_path)

    # 执行预测
    result = exe.run(program=infer_program,
                     feed={feeded_var_names[0]: img},
                     fetch_list=target_var)
    # 显示图片并输出结果最大的label
    lab = np.argsort(result)[0][0][-1]

    names = ['梨子', '樱桃', '哈密瓜', '胡萝卜', '黄瓜', '西瓜']

    # print('预测结果标签为：%d， 名称为：%s， 概率为：%f' % (lab, names[lab], result[0][0][lab]))
    return key_words[names[lab]]


if __name__ == '__main__':
    image_dirs = os.listdir('./images')
    success = 0
    failure = 0
    for image_dir in image_dirs:
        image_dir = os.path.join('./images', image_dir)
        if os.path.isdir(image_dir):
            images = os.listdir(image_dir)
            for image in images:
                image = os.path.join(image_dir, image)
                image_type = imghdr.what(image)
                if image_type != 'jpeg' and image_type != 'png':
                    print('error')
                else:
                    if infer(image) == os.path.split(os.path.split(image)[0])[1]:
                        print('success')
                        success = success + 1
                    else:
                        print('failure')
                        failure = failure + 1
                        print(image)
    print('成功:%d,失败:%d,成功率%f' % (success, failure, success / (success + failure)))
