from sklearn import svm
from sklearn import datasets
from sklearn.externals import joblib
import os

# load dataset
digits = datasets.load_digits()
X, y = digits.data, digits.target

# setup classifier
clf = svm.SVC(gamma=0.001, C=100.)

# train classifier
clf.fit(X, y)

# save classifier into a file
# os.chdir('../mlClassifier')
print '========================================='
print os.path.abspath(os.curdir)
print '========================================='
joblib.dump(clf, 'mlClassifiers/clf.pkl')

