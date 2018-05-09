import pandas as pd
import tensorflow as tf
import numpy as np
import sys
from sklearn import preprocessing
import os
import math as m
import csv

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
tf.logging.set_verbosity(tf.logging.ERROR)
#tf.logging.set_verbosity(tf.logging.ERROR)

LEARNING_RATE = 0.10
# Classifier par défaut


STEPS = 2500
NB_EVAL = 1
BATCH = 200
Length = 0
CSV_COLUMN_NAMES = ['case1', 'case2', 'case3', 'case4', 'case5',
                    'case6', 'case7', 'case8', 'case9', 'case10',
                    'case11', 'case12', 'case13', 'case14', 'case15',
                    'case16', 'case17', 'case18', 'case19', 'case20',
                    'case21', 'case22', 'case23', 'case24', 'case25', 'selected']

SPECIES = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13',
           '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25']
COLUMN_TO_NORMALIZE = ['case1', 'case2', 'case3', 'case4', 'case5',
                       'case6', 'case7', 'case8', 'case9', 'case10',
                       'case11', 'case12', 'case13', 'case14', 'case15',
                       'case16', 'case17', 'case18', 'case19', 'case20',
                       'case21', 'case22', 'case23', 'case24', 'case25']
feature_names = ['case1', 'case2', 'case3', 'case4', 'case5',
                 'case6', 'case7', 'case8', 'case9', 'case10',
                 'case11', 'case12', 'case13', 'case14', 'case15',
                 'case16', 'case17', 'case18', 'case19', 'case20',
                 'case21', 'case22', 'case23', 'case24', 'case25']


def load_data(filename_csv):
    """Returns the iris dataset as (train_x, train_y), (test_x, test_y)."""
    y_name = 'selected'
    df1 = pd.read_csv(filename_csv, names=CSV_COLUMN_NAMES, header=0, skiprows=0)
    # df2 = pd.read_csv("Agent1_to_train_4.csv", names=CSV_COLUMN_NAMES, header=0)

##    # Create x, where x the 'scores' column's values as floats
##    for column in COLUMN_TO_NORMALIZE:
##        x1 = df1[[column]].values.astype(float)
##        
##        # x2 = df2[[column]].values.astype(float)
##
##        # Create a minimum and maximum processor object
##        min_max_scaler1 = preprocessing.MinMaxScaler()
##        # min_max_scaler2 = preprocessing.MinMaxScaler()
##
##        # Create an object to transform the data to fit minmax processor
##        x_scaled1 = min_max_scaler1.fit_transform(x1)
##        # x_scaled2 = min_max_scaler2.fit_transform(x2)
##        print(x_scaled1)
##
##        # Run the normalizer on the dataframe
##        df1[column] = pd.DataFrame(x_scaled1)
##        # df2[column] = pd.DataFrame(x_scaled2)

    #df_norm = df1[df1.columns.difference(['selected'])]

    #768
    
##    for column in COLUMN_TO_NORMALIZE:
##        for index, row in df1.iterrows():
##            new_value=m.log(df1[column][index])
##            
##            df1.ix[index, column] = new_value
##            
##            new_value=(df1[column][index]-1)/3
##            
##            df1.ix[index, column] = new_value
##    print(df1)
        

    #df_norm.columns=COLUMN_TO_NORMALIZE

    #df1.update(df_norm)

    msk = np.random.rand(len(df1)) != 1
    train = df1
    test = df1
    dataPop = df1.pop(y_name)
##    train = df1[msk]
##    #test = df1[~msk]
##    test = df1[msk]

    # 1. Assign the DataFrame's labels (the right-most column) to train_label.
    # 2. Delete (pop) the labels from the DataFrame.
    # 3. Assign the remainder of the DataFrame to train_features
    train_feature, train_label = train, dataPop
    test_feature, test_label = test, dataPop

    return (train_feature, train_label), (test_feature, test_label)


def model_fn(features, labels, mode, params):
    """Model function for Estimator."""

    # Connect the first hidden layer to input layer
    # (features["x"]) with relu activation

    # All our inputs are feature columns of type numeric_column

    feature_columns = [
        tf.feature_column.numeric_column(feature_names[0]),
        tf.feature_column.numeric_column(feature_names[1]),
        tf.feature_column.numeric_column(feature_names[2]),
        tf.feature_column.numeric_column(feature_names[3]),
        tf.feature_column.numeric_column(feature_names[4]),
        tf.feature_column.numeric_column(feature_names[5]),
        tf.feature_column.numeric_column(feature_names[6]),
        tf.feature_column.numeric_column(feature_names[7]),
        tf.feature_column.numeric_column(feature_names[8]),
        tf.feature_column.numeric_column(feature_names[9]),
        tf.feature_column.numeric_column(feature_names[10]),
        tf.feature_column.numeric_column(feature_names[11]),
        tf.feature_column.numeric_column(feature_names[12]),
        tf.feature_column.numeric_column(feature_names[13]),
        tf.feature_column.numeric_column(feature_names[14]),
        tf.feature_column.numeric_column(feature_names[15]),
        tf.feature_column.numeric_column(feature_names[16]),
        tf.feature_column.numeric_column(feature_names[17]),
        tf.feature_column.numeric_column(feature_names[18]),
        tf.feature_column.numeric_column(feature_names[19]),
        tf.feature_column.numeric_column(feature_names[20]),
        tf.feature_column.numeric_column(feature_names[21]),
        tf.feature_column.numeric_column(feature_names[22]),
        tf.feature_column.numeric_column(feature_names[23]),
        tf.feature_column.numeric_column(feature_names[24])
    ]

    # Create the layer of input
    input_layer = tf.feature_column.input_layer(features, feature_columns)

    # first_hidden_layer = tf.layers.dense(features["x"], 25, activation=tf.nn.tanh)

    # Connect the second hidden layer to first hidden layer with relu
    second_hidden_layer = tf.layers.dense(
        input_layer, 150, activation=tf.nn.sigmoid)

    third_hidden_layer = tf.layers.dense(
        second_hidden_layer, 50, activation=tf.nn.sigmoid)

    fourth_hidden_layer = tf.layers.dense(
        third_hidden_layer, 25, activation=tf.nn.tanh)
    #
    # fifth_hidden_layer = tf.layers.dense(
    #     fourth_hidden_layer, 25, activation=tf.nn.tanh)

    # Connect the output layer to second hidden layer (no activation fn)
    logits = tf.layers.Dense(25)(fourth_hidden_layer)
    # Reshape output layer to 1-dim Tensor to return predictions
    predictions = {'class_ids': tf.argmax(input=logits, axis=1),
                   'probabilities': tf.nn.softmax(logits)}


    # Provide an estimator spec for `ModeKeys.PREDICT`.
    if mode == tf.estimator.ModeKeys.PREDICT:
        return tf.estimator.EstimatorSpec(
            mode=mode,
            predictions=predictions)

    # accuracy = tf.metrics.accuracy(labels=labels,
    #                                predictions=predictions['class_ids'])
    accuracy = tf.metrics.accuracy(labels=labels,
                                   predictions=tf.argmax(input=logits, axis=1))

    loss = tf.losses.sparse_softmax_cross_entropy(labels=labels, logits=logits)
    metrics = {'accuracy': accuracy}

    # savedata to hooks for graph in tensorboard with path to ./summaries
    tf.summary.scalar('accuracy', accuracy[1])
    tf.summary.scalar('loss', loss)

    # print(mode)

    # Calculate loss using mean squared error
    # Calculate root mean squared error as additional eval metric
    # loss = tf.losses.mean_squared_error(labels, predictions)
    if mode == tf.estimator.ModeKeys.EVAL:
        return tf.estimator.EstimatorSpec(
            mode=mode,
            loss=loss,
            eval_metric_ops=metrics,
            evaluation_hooks=[tf.train.SummarySaverHook(
                save_secs=0,
                summary_op=tf.summary.merge_all()
            )])

    optimizer = tf.train.GradientDescentOptimizer(
        learning_rate=params["learning_rate"])
    train_op = optimizer.minimize(
        loss=loss, global_step=tf.train.get_global_step())

    if mode == tf.estimator.ModeKeys.TRAIN:
        return tf.estimator.EstimatorSpec(
            mode=mode,
            loss=loss,
            train_op=train_op,
            eval_metric_ops=metrics,
            training_hooks=[tf.train.SummarySaverHook(
                save_steps=1,
                summary_op=tf.summary.merge_all()
            )])


def cnn_model_fn(features, labels, mode, params):
  """Model function for CNN."""
  # Input Layer
##  print(features["case1"].IteratorGetNext())
##  input_layer = tf.reshape(features, [-1, 5, 5, 1])

  feature_columns = [
        tf.feature_column.numeric_column(feature_names[0]),
        tf.feature_column.numeric_column(feature_names[1]),
        tf.feature_column.numeric_column(feature_names[2]),
        tf.feature_column.numeric_column(feature_names[3]),
        tf.feature_column.numeric_column(feature_names[4]),
        tf.feature_column.numeric_column(feature_names[5]),
        tf.feature_column.numeric_column(feature_names[6]),
        tf.feature_column.numeric_column(feature_names[7]),
        tf.feature_column.numeric_column(feature_names[8]),
        tf.feature_column.numeric_column(feature_names[9]),
        tf.feature_column.numeric_column(feature_names[10]),
        tf.feature_column.numeric_column(feature_names[11]),
        tf.feature_column.numeric_column(feature_names[12]),
        tf.feature_column.numeric_column(feature_names[13]),
        tf.feature_column.numeric_column(feature_names[14]),
        tf.feature_column.numeric_column(feature_names[15]),
        tf.feature_column.numeric_column(feature_names[16]),
        tf.feature_column.numeric_column(feature_names[17]),
        tf.feature_column.numeric_column(feature_names[18]),
        tf.feature_column.numeric_column(feature_names[19]),
        tf.feature_column.numeric_column(feature_names[20]),
        tf.feature_column.numeric_column(feature_names[21]),
        tf.feature_column.numeric_column(feature_names[22]),
        tf.feature_column.numeric_column(feature_names[23]),
        tf.feature_column.numeric_column(feature_names[24])
  ]

  # Create the layer of input
  input_layer = tf.feature_column.input_layer(features, feature_columns)
  input_layer = tf.reshape(input_layer, [-1, 5, 5, 1])

##  x = tf.Print(input_layer, [input_layer])
##
##  sess = tf.InteractiveSession()
##  sess.run(x)
##  print(x.eval())

  # Convolutional Layer #1
  conv1 = tf.layers.conv2d(
      inputs=input_layer,
      filters=5,
      kernel_size=[4, 4],
      padding="same",
      activation=tf.nn.relu)

  # Pooling Layer #1
  #pool1 = tf.layers.max_pooling2d(inputs=conv1, pool_size=[2, 2], strides=2)

  # Convolutional Layer #2 and Pooling Layer #2
  conv2 = tf.layers.conv2d(
      inputs=conv1,
      filters=5,
      kernel_size=[6, 6],
      padding="same",
      activation=tf.nn.relu)
  #pool2 = tf.layers.max_pooling2d(inputs=conv2, pool_size=[2, 2], strides=2)

  conv3 = tf.layers.conv2d(
      inputs=conv2,
      filters=5,
      kernel_size=[2, 2],
      padding="same",
      activation=tf.nn.relu)

  # Dense Layer
  pool2_flat = tf.reshape(conv3, [-1, 125])
  dense = tf.layers.dense(inputs=pool2_flat, units=125, activation=tf.nn.relu)
  dropout = tf.layers.dropout(
      inputs=dense, rate=0.4, training=mode == tf.estimator.ModeKeys.TRAIN)

  # Logits Layer
  logits = tf.layers.dense(inputs=dropout, units=25)

  prediction = tf.argmax(logits, 1)
  predictions = {
      # Generate predictions (for PREDICT and EVAL mode)
      "classes": prediction,
      # Add `softmax_tensor` to the graph. It is used for PREDICT and by the
      # `logging_hook`.
      "probabilities": tf.nn.softmax(logits, name="softmax_tensor")
  }

  if mode == tf.estimator.ModeKeys.PREDICT:
    return tf.estimator.EstimatorSpec(mode=mode, predictions=predictions)

  # Calculate Loss (for both TRAIN and EVAL modes)sparse_softmax_cross_entropy
  loss = tf.losses.sparse_softmax_cross_entropy(labels=labels, logits=logits)

  # Configure the Training Op (for TRAIN mode)
  if mode == tf.estimator.ModeKeys.TRAIN:
    optimizer = tf.train.GradientDescentOptimizer(learning_rate=params["learning_rate"])
    train_op = optimizer.minimize(
        loss=loss,
        global_step=tf.train.get_global_step())
    return tf.estimator.EstimatorSpec(mode=mode, loss=loss, train_op=train_op)

##  accuracy = tf.metrics.accuracy(
##          labels=labels, predictions=predictions["classes"])

  
  equality = tf.equal(prediction, labels)
  accuracy = tf.metrics.mean(tf.cast(equality, tf.float32))

  # Add evaluation metrics (for EVAL mode)
  eval_metric_ops = {
      "accuracy": accuracy}
  return tf.estimator.EstimatorSpec(
      mode=mode, loss=loss, eval_metric_ops=eval_metric_ops)


# Set model params
model_params = {"learning_rate": LEARNING_RATE}
# Instantiate Estimator after fonction model_fn
classifier = tf.estimator.Estimator(model_fn=cnn_model_fn, params=model_params, model_dir="ModelNN")
classifier2 = tf.estimator.Estimator(model_fn=cnn_model_fn, params=model_params, model_dir="ModelNN2")


def train_input_fn(features, labels, batch_size):
##    print((dict(features)))
##    print(features[features.isnull().any(axis=1)])
    dataset = tf.data.Dataset.from_tensor_slices((dict(features), labels))
    dataset = dataset.shuffle(buffer_size=1000).repeat(count=None).batch(batch_size)
    iterator = dataset.make_one_shot_iterator()
    batch_features, batch_labels = iterator.get_next()
    return batch_features, batch_labels


def eval_input_fn(features, labels=None, batch_size=None):
    """An input function for evaluation or prediction"""

    features = dict(features)
    if labels is None:
        # No labels, use only features.
        inputs = features
    else:
        inputs = (features, labels)

    # Convert inputs to a tf.dataset object.
    dataset = tf.data.Dataset.from_tensor_slices(inputs)

    # Batch the examples
    assert batch_size is not None, "batch_size must not be None"
    dataset = dataset.batch(batch_size)

    # Return the read end of the pipeline.
    return dataset.make_one_shot_iterator().get_next()

def load_model(model_dir):
    # Set model params
    model_params = {"learning_rate": LEARNING_RATE}
    # ecrase le classifier par défaut
    global classifier
    classifier = tf.estimator.Estimator(model_fn=model_fn, params=model_params, model_dir=model_dir)


def predict_with_model(grid_value):

    data = []
    all_data = grid_value
    data = all_data.split(",")
    data = list(map(int, data))

    grid = {
        'case1': [data[0]],
        'case2': [data[1]],
        'case3': [data[2]],
        'case4': [data[3]],
        'case5': [data[4]],
        'case6': [data[5]],
        'case7': [data[6]],
        'case8': [data[7]],
        'case9': [data[8]],
        'case10': [data[9]],
        'case11': [data[10]],
        'case12': [data[11]],
        'case13': [data[12]],
        'case14': [data[13]],
        'case15': [data[14]],
        'case16': [data[15]],
        'case17': [data[16]],
        'case18': [data[17]],
        'case19': [data[18]],
        'case20': [data[19]],
        'case21': [data[20]],
        'case22': [data[21]],
        'case23': [data[22]],
        'case24': [data[23]],
        'case25': [data[24]]
    }

    # PREDICTION
    predictions = classifier.predict(
        input_fn=lambda: eval_input_fn(grid,
                                       batch_size=BATCH))
    proba_list = []
    for pred_dict in predictions:
        for prob in pred_dict["probabilities"]:
            proba_list.append(prob)

    # listePrediction = list(predictions)
    # listePrediction =listePrediction[0]['probabilities']

    return proba_list


def train_with_model( filename_csv):

    # Call load_data() to parse the CSV file.
    (train_feature, train_label), (test_feature, test_label) = load_data(filename_csv)

    # Create feature columns for all features.
    my_feature_columns = []
    for key in train_feature.keys():
        my_feature_columns.append(tf.feature_column.numeric_column(key=key))


    # launch_mode = "train"
    STEPS_MAX = STEPS
    nb_step = int(round(STEPS_MAX / NB_EVAL))
    current_train = 0

    # TRAINING AND EVAL
    while (current_train != NB_EVAL):
        classifier.train(
            input_fn=lambda: train_input_fn(train_feature, train_label,
                                            batch_size=BATCH),
            steps=nb_step)


        # Evaluate the model.
        eval_result = classifier.evaluate(
            input_fn=lambda: eval_input_fn(test_feature, test_label,
                                           batch_size=BATCH))

        print(eval_result["accuracy"])
        current_train += 1


def predict_with_model2(grid_value):

    data = []
    all_data = grid_value
    data = all_data.split(",")
    data = list(map(int, data))

    grid = {
        'case1': [data[0]],
        'case2': [data[1]],
        'case3': [data[2]],
        'case4': [data[3]],
        'case5': [data[4]],
        'case6': [data[5]],
        'case7': [data[6]],
        'case8': [data[7]],
        'case9': [data[8]],
        'case10': [data[9]],
        'case11': [data[10]],
        'case12': [data[11]],
        'case13': [data[12]],
        'case14': [data[13]],
        'case15': [data[14]],
        'case16': [data[15]],
        'case17': [data[16]],
        'case18': [data[17]],
        'case19': [data[18]],
        'case20': [data[19]],
        'case21': [data[20]],
        'case22': [data[21]],
        'case23': [data[22]],
        'case24': [data[23]],
        'case25': [data[24]]
    }

    # PREDICTION
    predictions = classifier2.predict(
        input_fn=lambda: eval_input_fn(grid,
                                       batch_size=BATCH))
    proba_list = []
    for pred_dict in predictions:
        for prob in pred_dict["probabilities"]:
            proba_list.append(prob)

    # listePrediction = list(predictions)
    # listePrediction =listePrediction[0]['probabilities']

    return proba_list


def train_with_model2( filename_csv):

    # Call load_data() to parse the CSV file.
    (train_feature, train_label), (test_feature, test_label) = load_data(filename_csv)

    # Create feature columns for all features.
    my_feature_columns = []
    for key in train_feature.keys():
        my_feature_columns.append(tf.feature_column.numeric_column(key=key))


    # launch_mode = "train"
    STEPS_MAX = STEPS
    nb_step = int(round(STEPS_MAX / NB_EVAL))
    current_train = 0

    # TRAINING AND EVAL
    while (current_train != NB_EVAL):
        classifier2.train(
            input_fn=lambda: train_input_fn(train_feature, train_label,
                                            batch_size=BATCH),
            steps=nb_step)


        # Evaluate the model.
        eval_result = classifier2.evaluate(
            input_fn=lambda: eval_input_fn(test_feature, test_label,
                                           batch_size=BATCH))

        print(eval_result["accuracy"])
        current_train += 1


def predict2model(grid_value):
    pred1 = predict_with_model(grid_value)
    
    pred2 = predict_with_model2(grid_value)

    pred = pred1+pred2

    return pred
    

def yolo():
    print("yolo")


def main(args):
    # grid = {
    #     'case1': [1],
    #     'case2': [1],
    #     'case3': [1],
    #     'case4': [2],
    #     'case5': [1],
    #     'case6': [1],
    #     'case7': [1],
    #     'case8': [2],
    #     'case9': [1],
    #     'case10': [1],
    #     'case11': [1],
    #     'case12': [2],
    #     'case13': [1],
    #     'case14': [1],
    #     'case15': [1],
    #     'case16': [2],
    #     'case17': [1],
    #     'case18': [1],
    #     'case19': [1],
    #     'case20': [2],
    #     'case21': [1],
    #     'case22': [1],
    #     'case23': [1],
    #     'case24': [2],
    #     'case25': [2]
    # }
    if len(sys.argv) > 1:
        launch_mode = sys.argv[1]
        # print(launch_mode)
        # Directory Model + fichier d'entrainement
        if launch_mode == "train":
            train_with_model(sys.argv[2], sys.argv[3])
        # Directory Model + grid
        if launch_mode == "predict":
            data = []
            all_data = sys.argv[3]
            data = all_data.split(",")
            data = list(map(int, data))

            grid = {
                'case1': [data[0]],
                'case2': [data[1]],
                'case3': [data[2]],
                'case4': [data[3]],
                'case5': [data[4]],
                'case6': [data[5]],
                'case7': [data[6]],
                'case8': [data[7]],
                'case9': [data[8]],
                'case10': [data[9]],
                'case11': [data[10]],
                'case12': [data[11]],
                'case13': [data[12]],
                'case14': [data[13]],
                'case15': [data[14]],
                'case16': [data[15]],
                'case17': [data[16]],
                'case18': [data[17]],
                'case19': [data[18]],
                'case20': [data[19]],
                'case21': [data[20]],
                'case22': [data[21]],
                'case23': [data[22]],
                'case24': [data[23]],
                'case25': [data[24]]
            }

            # predict_with_model(sys.argv[2], sys.argv[3])
            return predict_with_model(sys.argv[2], grid)
    else:
        #train_with_model("100Games_Normalize.csv")
        #train_with_model2("100Games_Normalize_size.csv")
        #predict = predict2model("96,24,6,6,24,2,2,1,1,48,12,2,6,6,3,1,1,2,12,12,96,48,2,12,48")
        predict = predict_with_model2("96,24,6,6,24,2,2,1,1,48,12,2,6,6,3,1,1,2,12,12,96,48,2,12,48")
        #predict_train()
        print(predict)
        i=0
        best=0
        
        ind=0
        for value in predict:
            if value > best:
                best = value
                ind=i
            i+=1
        print("best: " + str(best) + ", case " + str(ind))
        #print(probabilities)

if __name__ == '__main__':
    # tf.logging.set_verbosity(tf.logging.INFO)
    # parser = argparse.ArgumentParser()
    # parser.add_argument('--batch_size', default=150, type=int, help='batch size')
    # parser.add_argument('--train_steps', default=STEPS, type=int, help='number of training steps')
    # Learning rate for the model
    #
    # tf.app.run(main)

    # predict_with_model("ModelNN", grid)
    tf.app.run(main)

def Maboucle():
    grid = "1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7"
    predict_with_model(grid)
