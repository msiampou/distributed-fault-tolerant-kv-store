# Distributed-Fault-Tolerant-KV-Store

### Data Creation

##### Language

Python3

##### Execution Example
```
python3 createData.py -k keyFile.txt -n 10000 -d 3 -l 4 -m 3
```

##### Arguments

`[-n]`​ indicates the number of lines (i.e. separate data) that we would like to generate.
`[-d]` ​is the maximum level of nesting. The value of `[d]` should be >= 0, otherwise a ValueError is raised.
`[-l]` ​is the maximum length of a string value. In case of an integer, float or double type, I assumed that this refers to the number of digits. The value of `[l]` should be > 0, otherwise a ValueError is raised.
`[-m​]` is the maximum number of keys inside each value. The value of `[m]` should be >= 0, to create at least 1 key-value pair.

##### Implementation Details

The data creation script creates totally random strings that contain nested key-value pairs. Both keys and their values are generated based on the content of the keyFile.txt, except from the top level keys that have the follwing format: ``` key0, key1 etc ``` .  The script supports only 4 types of values: int, float, string and double. Finally, since everything is generated randomly, there might exist a contradiction between the data. For example:

```
person21: { key0: 2619; key1: {}; key2: { postalcode : teen; postalcode : {} } }
```

In this case we have generated to different values for the same key. This case is latter handled by the KVStore.
Last but not least, the keys generated do not contain " ".

##### Excecution examples

```
person20: { key0: {}; key1: {}; key2: { name : jvnh; height : 14.09 } }
person21: { key0: 2619; key1: {}; key2: { postalcode : teen } }
person22: { key0: {}; key1: 8266; key2: 1984 }
person23: { key0: 8731; key1: {}; key2: { height : 16.72; profession : vkqo } }
```

### KV Store

##### Language

C++ 11

##### Compilation && Excecution

To run a server:
```
cd server
sh ./compile.sh
./kvServer -a [IP] -p [port]
```

To run a broker:
```
cd broker
sh ./compile.sh
./kvBroker -s [serversFile] -i [indexFile] -k [replicationFactor]
```

In order for the excecution to be successful, the *servers* should be launched first. After each server establishes connection, the *broker* will be able to run and connect to each one of the servers.

##### Arguments

`[-a]`: IP address. Use localhost in case of local connection.
`[-p]`: Port number
`[-s]`: serversFile that contains the IPs and ports of each server.
`[-i]`: The data file that has been generated using the createData script.
`[-k]`: Replication factor indicates how many servers will index the same data. k must be less or equal to the number of servers in the serverfile.

##### Implementation Details

###### KV Broker

The kvBroker is implemented as a class named `client`. The excecution of the kvBroker happens in the following steps: 
```
(1) The KvBroker parses the serverFile and tries to connect to each one of the servers. 
(2) The KvBroker parses the indexFile and sends a PUT request to maximum k servers to index the data.
(3) If less than [k] servers are available KvBroker outputs a warning that does not guarantee correctness of the upcoming requests.
(4) The KvBroker receives a user request and sends it to all of the servers. The available commands are {PUT key, GET key, QUERY key, DELETE key, E}.
(5) The KvBroker outputs the results it received from each server.
(6) Steps (4)-(5) continue excecuting until the user types an {E} request, which terminates all of the connections.
```
###### KV Server

The kvServer is implemented as a class named `server`. The excecution of the kvBroker happens in the following steps:
```
(1) The kvServer creates a socket and connects.
(2) The kvServer receives PUT requests to index some data in its an n-gram Trie.
    * kvServer parses each string it receives and creates keypaths. If one of the string doen't have the appropriate format, server terminates immediately.
      For example:
        For the following string: person1 : { key1: 22 ; key2: X }
        The kvServer would generate the following keypaths:
        - person1 key1 22
        - person1 key2 X
    * The kvServer checks if each keypath is valid, depending on what is stored in the Trie so far.
    * If the keypath is valid, it is inserted into Trie, otherwise it is discarded.
(4) After indexing is completed, server receives requests and returns their results to the broker. In case of unkonown request, server returns "Unknown Request".
(5) Server terminates when it receives and *exit* signal from the broker.
```

###### n-gram Trie

Each kvServer indexes the data it receives using a single n-gram Trie. An n-gram Trie is an alteration of the usual Trie, which - instead of a char - strores a whole string in each of its node. For the purposes of this project, such a Trie implementation proves to be extremely helful to index and query the data. An example of an n-gram Trie is the following:

```
$ PUT person1 : { key1: 22 ; key2: X }

person1
  |
 key1 - key2
  |      |
  22     X

$ PUT person2 : {}

person1       -     person2
  |                    |
 key1 - key2          \0
  |      |
  22     X
```

The implementation of the Trie lies under a class named 'trie'.
The implementation of the Trie is fully templated and can be used to store both chars and strings.

##### Files && Folders

```
- data: Contains the files that are given as arguments.
- library
  - io
    - command_handler: Contains the command_handler's header files.
    - parser: Contains the parser's header files. 
  - network
    - broker: Contains the implementation of the kvBroker.
    - socket: Contains the implementation of a TCP socket.
    - server: Contains the implementation of the kvServer.
  - structures: Contains the implementation of the n-gram Trie.
  - src
    - command_handler: Contains the implementation of various functions that assist to the requests execution.
    - parser: Contains the implementation of various parsers (file parser, json parser etc). 
  - test
    - broker: Contains the main function that executes kvBroker.
    - server: Contains the main function that executes kvServer.
```

##### Notes
- The execution of the program has been tested with maximum 3 servers and 1 client.
- It is assumed that the length of each entry will not exceed the maximum socket size.
- Connections are continues, meaning that if a server disconects, it cannot reconnect.
- When k or more servers are down, the program continues the exececution, but it prints a warning.
- Since a validation check is performed for each one of the entries, that may slow down things a bit for deep nesting.
- You can execute each command by writting either everything on camel case or only the first letter. For example: `GET person1000 && Get person1000` are both acceptable.
- This program has been developed in Manjaro Linux enviroment.

##### Author
Siampou Maria Despoina
cs2200017@di.uoa.gr
