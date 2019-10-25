# Elrond Runtime Application for Linux

Runtime for Elrond Modules for Linux based platforms. The runtime application is the engine of projects based on Elrond Modules. The application runtime is responsible for load and initialize the modules instances and control to everything runs fine.

## Build and run

First of all, you will need of build-essentials package for you distro linux. The requirements are:
 - **GNU G++**;
 - **GNU Make**;
 - **GNU ar**;
 - **Git**;

After the repository clone, will need initialize the git submodule:
```
$ git submodule init
$ git submodule update
```

To build and run will be need a configuration file like:
```json
{
    "modules":{
        "test": "elrond::Example"
    },
    "init":{
        "test":{
            "allowLoop": true,
            "asyncLoop": true,
            "timeLoop": 1000
        }
    }
}
```

Saved as `config.json`, to run just type:
```
$ make run cfg=config.json
```

The output are:
```
./build/elrond-runtime config.json
 * Starting Elrond Runtime for Linux v0.0-alpha (API 0.0) by Edwino Stein <edwino.stein@gmail.com>...
 * Loading modules (1)...
  #1: Loading "test" from "elrond::Example"...
      Loaded: Example v1.0 (API 0.0) by Edwino Stein <edwino.stein@gmail.com>
      Created instance "test" of module "Example"
 * Initializing modules instances (1)...
  #1: Initializing instance "test"...
Example::onInit
 * Starting all modules instances...
Example::onStart
 * Application running (CTRL+C to stop)...
Example::loop (async)
Example::loop (async)
Example::loop (async)
 * Received Signal INT (2): Interrupt
 * Stopping all modules instances...
Example::onStop
```
