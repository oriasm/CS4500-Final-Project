### Introduction
The eau2 system that we are implementing is a distributed key/value store to be utilized for data anlytics. The system will read in data in SoR format and cannot be changed once it has been read. Once the data is read in, it can be processed in whichever way the customer desires.

### Architecture
There are three main portions to the eau2 system, a distributed KV store running on each node also handling networking protocols, an abstraction of distributed arrays and data frames which allows for the data to be properly accessed, and then an application layer allowing for additional code to be written to interact with the underlying data. For the KV layer, we will always know how many nodes there are and the KV pair is used to determine where the data is stored. For the middle layer, certain data structures are being built in-house such as the distributed arrays and data frames, this way we can optimize for performance and the few data types we must handle. For the application layer, all that needs to be done is implementing an easy to use interface that allows for users to interact with the data, without modifying the underlying storage.

### Implementation
We can reuse some code given to us from previous projects such as the `helper.h`, `object.h`, and `string.h` files. The most important classes to design properly are the DataFrame and Array classes, as they must work in unison in order to properly manage and access all of the data read in. Additionally, the Network class will need to be efficiently implemented without errors because that is how the nodes will communicate with one another to ensure that state is consistent. The final portion of the project that must be focused on is the Sorer, which is the application that will read in the SoR files. This too must be very efficient as the potential for large SoR files is very high, so we must be able to handle and break down large files and then read them in quickly.

### Use Cases
The use of this system will be reading in data and then having additional applications to then process it as desired.

`./eau2 example_data.sor` read in a SoR file (assume that file is columns of 5 integers)

`Schema *schema = new Schema("IIIII");` create a Schema to be used 

`DataFrame *df = new DataFrame(*schema);` create a new DataFrame based upon the Schema read in

### Open Question
1. How important is speed to our client?
2. What should happen in the case some of the nodes crash?
3. Will we have to support other files besides SoR in the future?

### Status
1. Serialization when communicating between nodes
2. Linking the Sorer with the rest of the project
3. Debugging memory leaks