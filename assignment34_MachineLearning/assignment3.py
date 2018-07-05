import pandas as pd
import numpy as np
import csv
from sklearn import tree
from sklearn.model_selection import train_test_split
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
import graphviz # graph module

data = pd.read_csv('as3.csv')
data1 = np.array(data)

X = list(map(lambda x: x[1:8], data1))
Y = list(map(lambda x: x[0], data1))

# random split
x_train, x_test, y_train, y_test = train_test_split(X, Y, test_size=0.2, random_state=42)

# 하단에 작성
#get features
with open('as3.csv',mode='r', encoding='utf-8') as f:
    reader = csv.reader(f)
    for row in reader:
        feature_names = row
        break;


for i in range(5,6):
    #train
    criterion = 'entropy'
    clf = tree.DecisionTreeClassifier(criterion=criterion, max_depth=i)
    clf = clf.fit(x_train, y_train)

    #predict
    pred = clf.predict(x_test)

    #calculate scores
    prec = precision_score(y_test, pred, average='macro')
    rec = recall_score(y_test, pred, average='macro')
    f1 = f1_score(y_test, pred, average='macro')
    print("criterion: %s" %criterion)
    print("max_depth: %d" %i)
    print("Precision_score:", prec)
    print("Recall_score:",rec)
    print("F1_score:",f1)

    #feature importances
    for i in range(1,8):
        print(feature_names[i])
        print(clf.feature_importances_[i-1])
        print('-----------------------------')
    print('-------------------------')

    #print graph
    dot_data = tree.export_graphviz(clf, out_file=None,
                                    feature_names=feature_names[1:8],
                                    class_names=feature_names[0],
                                    filled=True, rounded=True,
                                    special_characters=True)

    graph = graphviz.Source(dot_data)
    graph.render(filename='DecisionTree.gv',directory='.',view=True)
