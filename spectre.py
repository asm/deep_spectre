#!/usr/bin/env python3

import deep_spectre
import numpy as np
import keras
from keras.models import Sequential, Model
from keras.layers import Dense
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

# First, build our training data --
# Our training data shape will be [n, 256] where n is the number of samples
# The target data [n, y] is one hot encoded where y is the number of unique training chars.

n_iterations = 1000
train_str = deep_spectre.train_str()
n_chars = len(train_str)
n_samples = n_iterations * n_chars
secret_chars = list(map(lambda x: ord(x), list(train_str)))
unique_chars = np.unique(secret_chars)
char_to_idx_map = {k: v for v, k in enumerate(unique_chars)}
idx_to_char_map = {v: k for v, k in enumerate(unique_chars)}
data_x = np.zeros([n_samples, 256])
data_y = np.zeros([n_samples, len(unique_chars)])

print("Collecting training data...")
for i in range(n_iterations):
    for j, val in enumerate(secret_chars):
        data_x[i * n_chars + j, :] = deep_spectre.train(j);
        data_y[i * n_chars + j, char_to_idx_map[val]] = 1

print("Scaling data between 0-1...")
scaler = StandardScaler()
data_x = scaler.fit_transform(data_x)

x_train, x_test, y_train, y_test = train_test_split(data_x, data_y)

# Let's build a simple deep model --
model = Sequential()
model.add(Dense(200, input_shape = (256,), activation = 'relu'))
model.add(Dense(150, activation = 'relu'))
model.add(Dense(100, activation = 'relu'))
model.add(Dense(len(unique_chars), activation = 'softmax'))

model.compile(loss = 'categorical_crossentropy',
              optimizer = 'adam',
              metrics = ['accuracy'])

print("Training deep model...")
model.fit(x_train, y_train, batch_size = 32, epochs = 10, validation_data = (x_test, y_test))

# Now, read the secret and pass the timing results through our model --
secret_len = 40
x_message = np.zeros([secret_len, 256])
for i in range(secret_len):
    x_message[i, :] = deep_spectre.read(i)

y_pred = model.predict(scaler.transform(x_message))
pred_chars = np.argmax(y_pred, axis=1)
message = ''.join(list(map(lambda x: chr(idx_to_char_map[x]), pred_chars)))

print("The secret message is:", message)

