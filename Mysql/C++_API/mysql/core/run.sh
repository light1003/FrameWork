g++ test.cc mysql_op.cc ../utils/byte2binarystring.cc ../utils/splitstr.cc ../utils/time.cc -I. `mysql_config --cflags --libs` -std=c++11
