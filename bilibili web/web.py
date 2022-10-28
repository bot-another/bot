from flaskr import create_app
from flask_cors import CORS
if __name__ == '__main__':
    app = create_app()
    #CORS(app, resoures = r'/*')
    app.run(host='0.0.0.0', port=5000)
