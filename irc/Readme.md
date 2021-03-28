# Change IRC Connection info

In order to make the bot connect to your irc server/channel you must edit the main.cpp file at line: 41.


# Compile

```
cd source/diceparser
mkdir build 
cd build

cmake .. -DBUILD_IRC=ON 
make -j8
```



# Run it 

after 30second the bot must appears in the list of member of the channel.


# The IRC protocol 
IRC is managed by hand and its support is very limited.
