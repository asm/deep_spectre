# Deep Spectre

Deep Spectre is a deep learning side channel privileged memory reader heavily based on the PoC found [here](https://gist.github.com/ErikAugust/724d4a969fb2c6ae1bbd7b2a9e3d4bb6).  I've written a [Medium post](https://medium.com/@syne/exploiting-spectre-with-deep-learning-d8ec2ba4c8ca) explaining the deep learning code and you can read more about Spectre in 
[CVE-2017-5753](https://nvd.nist.gov/vuln/detail/CVE-2017-5753) and [CVE-2017-5715](https://nvd.nist.gov/vuln/detail/CVE-2017-5715) or check out the [whitepaper](https://spectreattack.com/spectre.pdf) and [Google Project Zero post](https://googleprojectzero.blogspot.com/2018/01/reading-privileged-memory-with-side.html).

### Installing
The [Python 3 C API](https://docs.python.org/3/c-api/index.html) is used to glue the PoC code to the  [Keras](https://keras.io/) deep learning API.  Thus, you'll have to compile and install a native module.  Simply run:

```sh
python setup.py install
```

After installing the packages listed in ```requirements.txt```, start the program and you should see the results below

```sh
./spectre.py
Using TensorFlow backend.
Collecting training data...
Scaling data between 0-1...
Training deep model...
Train on 48000 samples, validate on 16000 samples
Epoch 1/10
48000/48000 [==============================] - 4s 83us/step - loss: 2.9168 - acc: 0.3363 - val_loss: 0.7985 - val_acc: 0.8276
Epoch 2/10
48000/48000 [==============================] - 4s 73us/step - loss: 0.4543 - acc: 0.9007 - val_loss: 0.3505 - val_acc: 0.9204
Epoch 3/10
48000/48000 [==============================] - 4s 75us/step - loss: 0.2802 - acc: 0.9367 - val_loss: 0.2825 - val_acc: 0.9335
Epoch 4/10
48000/48000 [==============================] - 3s 73us/step - loss: 0.2516 - acc: 0.9441 - val_loss: 0.2948 - val_acc: 0.9293
Epoch 5/10
48000/48000 [==============================] - 4s 73us/step - loss: 0.2368 - acc: 0.9451 - val_loss: 0.2640 - val_acc: 0.9361
Epoch 6/10
48000/48000 [==============================] - 4s 73us/step - loss: 0.2320 - acc: 0.9460 - val_loss: 0.2765 - val_acc: 0.9360
Epoch 7/10
48000/48000 [==============================] - 3s 73us/step - loss: 0.2405 - acc: 0.9458 - val_loss: 0.2588 - val_acc: 0.9376
Epoch 8/10
48000/48000 [==============================] - 4s 74us/step - loss: 0.2324 - acc: 0.9468 - val_loss: 0.2502 - val_acc: 0.9403
Epoch 9/10
48000/48000 [==============================] - 4s 73us/step - loss: 0.2269 - acc: 0.9474 - val_loss: 0.2452 - val_acc: 0.9408
Epoch 10/10
48000/48000 [==============================] - 3s 72us/step - loss: 0.2277 - acc: 0.9467 - val_loss: 0.2663 - val_acc: 0.9392
The secret message is: The Magic Words are Squeamish Ossifrage.
```

License
----
MIT
