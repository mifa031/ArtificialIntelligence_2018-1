import numpy
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout
from keras.layers import Flatten
from keras.layers.convolutional import Conv2D
from keras.layers.convolutional import MaxPooling2D
from keras.utils import np_utils
from keras import backend as K
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score

K.set_image_dim_ordering('th')
#fix random seed for reproducibility
seed = 7
numpy.random.seed(seed)

#load data
(X_train, y_train), (X_test, y_test) = mnist.load_data()

#reshpae to be [samplses][pixels][width][height]
X_train = X_train.reshape(X_train.shape[0], 1, 28, 28).astype('float32')
X_test = X_test.reshape(X_test.shape[0], 1, 28, 28).astype('float32')

#normalize inputs from 0-255 to 0-1
X_train = X_train / 255
X_test = X_test / 255

#one hot encode outputs
y_train = np_utils.to_categorical(y_train)
y_test = np_utils.to_categorical(y_test)

#total number of class
num_classes = y_test.shape[1]




#define model
def cnn_model():
    model = Sequential()
    model.add(Conv2D(30,(5,5),input_shape=(1,28,28),activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(Dropout(0.5)) #overfitting 방지
    model.add(Flatten())  # 1차원화 시키기(펼치기), 전결합층에 넣기전에 필요한 작업
    model.add(Dense(50,activation='relu')) # Dense로 전결합층의 심층 신경망 생성
    model.add(Dense(num_classes, activation='softmax')) #softmax로 클래스 분류
    #compile model
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model

#build the model
model = cnn_model()
#fit the model (train 5000 set of train data, validate on 1000 set of validation data)
model.fit(X_train[:5000], y_train[:5000], validation_data=(X_train[59000:], y_train[59000:]), epochs=10, batch_size=200, verbose=1)
#final evaluation of the model (on 1000 set of test data)
scores = model.evaluate(X_test[9000:], y_test[9000:], verbose=1)

#prediction (on 1000 set of test data)
Y_pred = model.predict(X_test[9000:], verbose=1)
y_pred = numpy.argmax(Y_pred, axis=1)

#Calculate scores (on 1000 set of test data)
print("Large CNN Error: %.2f%%" %(100-scores[1]*100))
prec = precision_score(numpy.argmax(y_test[9000:],axis=1), y_pred, labels=[0,1,2,3,4,5,6,7,8,9], average=None)
rec = recall_score(numpy.argmax(y_test[9000:], axis=1), y_pred, labels=[0,1,2,3,4,5,6,7,8,9], average=None)
f1 = f1_score(numpy.argmax(y_test[9000:], axis=1), y_pred, labels=[0,1,2,3,4,5,6,7,8,9], average=None)

print("Precision Score :"); print(prec)
print("Recall Score :"); print(rec)
print("F1 Score :"); print(f1)

