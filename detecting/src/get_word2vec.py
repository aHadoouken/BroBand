import socket
import gensim
import numpy as np
import os

def get_word2vec(text, model):

    myvector = np.zeros(shape = (100))

    for word in text.split(' '):
        try:
            myvector += model.wv[word]
        except:
            continue
    
    return myvector

if __name__ == '__main__':

    word2vec_porn = gensim.models.Word2Vec.load(os.path.abspath('../model/word2vec/porn_word2vec.model'))
    
    # создание сокета для TCP/IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Привязываем сокет к порту
    server_address = ('localhost', 9090)
    sock.bind(server_address)

    # слушаем
    sock.listen(1)

    # ждем соединения
    while True:

        connection, addr = sock.accept()

        try:
            while True:

                data = connection.recv(1024)
                print(data.decode())

                if data:
                    vec = ' '.join(list(map(str, get_word2vec(str(data.decode()), word2vec_porn))))
                    connection.sendall(vec.encode('utf-8'))
                    del vec
                    del data
                else:
                    break

        finally:
            connection.close()
