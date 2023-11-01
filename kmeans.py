from sklearn.cluster import KMeans
import pandas as pd
X = pd.read_csv("/home/ankhanh/learning/data_mining/data/beijing_POI_18f.csv")
X = 
kmeans = KMeans(n_clusters=5, random_state=0).fit(X)
print('Centers found by scikit-learn:')
print(kmeans.cluster_centers_)
pred_label = kmeans.predict(X)
kmeans_display(X, pred_label)