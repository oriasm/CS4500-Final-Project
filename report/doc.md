### Introduction
The eau2 system that we are implementing is a distributed key/value store to be utilized for data anlytics. The system will read in data in SoR format and cannot be changed once it has been read. Once the data is read in, it can be processed in whichever way the customer desires.

### Architecture
There are three main portions to the eau2 system, a distributed KV store running on each node also handling networking protocols, an abstraction of distributed arrays and data frames which allows for the data to be properly accessed, and then an application layer allowing for additional code to be written to interact with the underlying data. For the KV layer, we will always know how many nodes there are and the KV pair is used to determine where the data is stored. For the middle layer, certain data structures are being built in-house such as the distributed arrays and data frames, this way we can optimize for performance and the few data types we must handle. For the application layer, all that needs to be done is implementing an easy to use interface that allows for users to interact with the data, without modifying the underlying storage.

### Implementation
We can reuse some code given to us from previous projects such as the `helper.h`, `object.h`, and `string.h` files. The most important classes to design properly are the DataFrame and Array classes, as they must work in unison in order to properly manage and access all of the data read in. Additionally, the Network class needs to be efficiently implemented without errors because that is how the nodes will communicate with one another to ensure that state is consistent. The final portion of the project that must be focused on is the Sorer, which is the application that will read in the SoR files. This too must be very efficient as the potential for large SoR files is very high, so we must be able to handle and break down large files and then read them in quickly. 

We chose to incorporate another team's Sorer (danyth) which was written in C++ and can be found in the `src/sorer` directory. We added an additional file to link their helper classes with our dataframe. `src/sorer/sorer.h` takes in a filename, parses it and returns a `Dataframe` object. Our `sorer.h` file works similarly to the `main.cpp` file that was originally created for the sor adaptor, but we modified it to access each element in the .sor file, as opposed to querying it for a specific row and column or column type.

Our network layer contains two classes, ClientSocket and ServerSocket which both handle listening on a certain IP and port. They each have specialized functions that facilitate the registration of mulitple clients to one server. Each class creates a thread to listen for messages on all the relevant file descriptors, while the main thread accepts commands from the user to send messages and display them. 

### Use Cases
The use of this system will be reading in data and then having additional applications to then process it as desired.

`./eau2 example_data.sor` read in a SoR file and create a Dataframe

` DataFrame *df = get_dataframe("example_data.sor, 0, 500);` internal command to sor adaptor that returns a dataframe

`Schema *schema = new Schema("IIII")` create a new schema of just integers

`DataFrame *df = new DataFrame(*schema);` create an empty DataFrame based upon the Schema read in

### Open Questions
1. How important is speed to our client?
2. What should happen in the case some of the nodes crash?
3. Will we have to support other files besides SoR in the future?

### Status
For the assignment, the work we have implemented and tested thus far includes Dataframe and its subsequent parts (Schema, Column, Row..). Further, we have completed the work on the Sorer adaptor which successfully converts .sor files to Dataframes. Auxilary classes for networking are also implemented. We will need to do additional work to deserialize messages from node to node. Right now our network interface simply prints messages to STDOUT. As it is unclear exactly how we would like to process those messages, this will be addressed in the coming weeks. It will require some rework of the network interface. 