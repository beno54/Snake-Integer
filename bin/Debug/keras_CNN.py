import math as m
import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import Dropout
from keras.layers import Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.models import model_from_json
import keras
from sklearn.preprocessing import LabelEncoder,OneHotEncoder
from keras import backend as K
import matplotlib.pyplot as plt

CSV_COLUMN_NAMES = ['case1', 'case2', 'case3', 'case4', 'case5',
                    'case6', 'case7', 'case8', 'case9', 'case10',
                    'case11', 'case12', 'case13', 'case14', 'case15',
                    'case16', 'case17', 'case18', 'case19', 'case20',
                    'case21', 'case22', 'case23', 'case24', 'case25', 'selected']

loaded_model_case=0
loaded_model_nb=0

def load_data(filename_csv):
    train = pd.read_csv(filename_csv, names=CSV_COLUMN_NAMES, header=0, skiprows=0).values
    train_feature = train[:5000, :25].reshape(5000,5, 5,1).astype( 'float32' )
    train_label = train[:5000,25:]
    return (train_feature, train_label)

def yolo():
    print("yolo")

def load_models():
    global loaded_model_case
    global loaded_model_nb

    print("load")
    # load case model
    # load json and create model
    json_file = open('keras_models/Model_case/model.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model_case = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model_case.load_weights("keras_models/Model_case/model.h5")

    # load nb model
    # load json and create model
    json_file = open('keras_models/Model_nb/model.json', 'r')
    loaded_model_json = json_file.read()
    json_file.close()
    loaded_model_nb = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model_nb.load_weights("keras_models/Model_nb/model.h5")
    

def predict_2_models(grid):
    predict1 = predict_with_model(grid)
    predict2 = predict_nb_with_model(grid)

    return (predict1+predict2)

def predict_with_model(grid):
    global loaded_model_case

    data = []
    all_data = grid
    data = all_data.split(",")
    data = list(map(int, data))
    input_data=[]
    for i in range(0, 5):
        row=[]
        for j in range(0, 5):
            row.append(data[i*5+j])
        input_data.append(row)
    
    
    input_data = np.array(input_data)
    input_data = input_data[:, :25].reshape(1,5, 5,1).astype( 'float32' )

    predictions = loaded_model_case.predict(input_data)

    #print("from py:", predictions[0])
    
##    for row in predictions:
##        best = 0
##        indice = 0
##        i=0
##        for score in row:
##            if (score > best):
##                best = score
##                indice = i
##            i+=1
##        print("prediction: case", indice, " avec", best, "%.")
    return list(predictions[0])

def predict_nb_with_model(grid):
    global loaded_model_nb

    data = []
    all_data = grid
    data = all_data.split(",")
    data = list(map(int, data))
    input_data=[]
    for i in range(0, 5):
        row=[]
        for j in range(0, 5):
            row.append(data[i*5+j])
        input_data.append(row)
    
    
    input_data = np.array(input_data)
    input_data = input_data[:, :25].reshape(1,5, 5,1).astype( 'float32' )

    predictions = loaded_model_nb.predict(input_data)

    #print("from py:", predictions[0])
    
##    for row in predictions:
##        best = 0
##        indice = 0
##        i=0
##        for score in row:
##            if (score > best):
##                best = score
##                indice = i
##            i+=1
##        print("prediction: case", indice, " avec", best, "%.")
    return list(predictions[0])

def learning_case(file_to_learn):
   # Call load_data() to parse the CSV file.
  (train_feature, train_label) = load_data(file_to_learn)
  model = Sequential()
  model.add(Conv2D(25, (2, 2), activation='relu', input_shape=(5,5,1)))
  model.add(Conv2D(25, (2,2), activation='relu'))
  #model.add(MaxPooling2D(pool_size=(2, 2)))
  model.add(Dropout(0.25))

  #Flattening is converting CNN output to be input of  ANN
  model.add(Flatten())
  model.add(Dense(1000, activation='sigmoid'))
  model.add(Dense(500, activation='sigmoid'))
  model.add(Dense(250, activation='sigmoid'))
  model.add(Dense(25, activation='softmax'))

  #Creation of the model
  model.compile(loss='sparse_categorical_crossentropy', optimizer=keras.optimizers.SGD(lr=0.01), metrics=['accuracy'])
  print ('MODEL',model.summary())

  #Learning part
  print ('LEARNING')
  history = model.fit(train_feature, train_label, validation_split=0.33, shuffle = True, epochs=25, batch_size=50,verbose=2)
   
  # summarize history for accuracy
  plt.subplot('211')
  plt.plot(history.history['acc'])
  plt.plot(history.history['val_acc']) 
  plt.title('model accuracy')
  plt.ylabel('accuracy')
  plt.xlabel('epoch')  
  plt.legend(['train', 'test'], loc='upper left')

  # summarize history for loss
  plt.subplot('212')
  plt.plot(history.history['loss'])
  plt.plot(history.history['val_loss'])
  plt.title('model loss')
  plt.ylabel('loss')
  plt.xlabel('epoch')  
  plt.legend(['train', 'test'], loc='upper left')
  plt.show()

  # serialize model to JSON
  model_json = model.to_json()
  with open("keras_models/Model_case/model.json", "w") as json_file:
      json_file.write(model_json)
  # serialize weights to HDF5
  model.save_weights("keras_models/Model_case/model.h5")
  print("Saved model to disk")
 
  # later...

##  predictions = model.predict(train_feature)
##  for row in predictions:
##      best = 0
##      indice = 0
##      i=0
##      for score in row:
##          if (score > best):
##              best = score
##              indice = i
##          i+=1
##      print("prediction: case", indice, " avec", best, "%.")

def learning_nb(file_to_learn):
   # Call load_data() to parse the CSV file.
  (train_feature, train_label) = load_data(file_to_learn)
  model = Sequential()
  model.add(Conv2D(25, (4, 4), activation='relu', input_shape=(5,5,1)))
  model.add(Conv2D(50, (2, 2), activation='relu'))
  #model.add(Conv2D(250, (2,2), activation='relu'))
  model.add(Dropout(0.5))

  #Flattening is converting CNN output to be input of  ANN
  model.add(Flatten())
  model.add(Dense(100, activation='sigmoid'))
  model.add(Dropout(0.5))
  model.add(Dense(50, activation='tanh'))
  model.add(Dropout(0.5))
  model.add(Dense(25, activation='tanh'))
  model.add(Dropout(0.5))
  model.add(Dense(12, activation='softmax'))

  #Creation of the model
  model.compile(loss='sparse_categorical_crossentropy', optimizer=keras.optimizers.SGD(lr=0.1), metrics=['accuracy'])
  print ('MODEL',model.summary())

  #Learning part
  print ('LEARNING')
  history = model.fit(train_feature, train_label, validation_split=0.33, shuffle = True, epochs=20, batch_size=50,verbose=2)
   
  # summarize history for accuracy
  plt.subplot('211')
  plt.plot(history.history['acc'])
  plt.plot(history.history['val_acc']) 
  plt.title('model accuracy')
  plt.ylabel('accuracy')
  plt.xlabel('epoch')  
  plt.legend(['train', 'test'], loc='upper left')

  # summarize history for loss
  plt.subplot('212')
  plt.plot(history.history['loss'])
  plt.plot(history.history['val_loss'])
  plt.title('model loss')
  plt.ylabel('loss')
  plt.xlabel('epoch')  
  plt.legend(['train', 'test'], loc='upper left')
  plt.show()

  # serialize model to JSON
  model_json = model.to_json()
  with open("keras_models/Model_nb/model.json", "w") as json_file:
      json_file.write(model_json)
  # serialize weights to HDF5
  model.save_weights("keras_models/Model_nb/model.h5")
  print("Saved model to disk")

  predictions = model.predict(train_feature)
  print(train_label)
##  for row in predictions:
##      best = 0
##      indice = 0
##      i=0
##      for score in row:
##          if (score > best):
##              best = score
##              indice = i
##          i+=1
##      print("prediction: case", indice, " avec", best, "%.")

    
if __name__ == '__main__':
    #learning_case("100Games_Normalize.csv")
    learning_nb("100Games_Normalize_size.csv")
    #load_models()
    #print(predict_2_models("48,6,24,6,96,3,1,1,1,3,3,1,3,3,1,24,1,2,2,2,1,1,1,6,48"))
    #predict_with_model("48,6,24,6,96,3,1,1,1,3,3,1,3,3,1,24,1,2,2,2,1,1,1,6,48")
