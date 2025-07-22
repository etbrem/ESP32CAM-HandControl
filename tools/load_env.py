import os
import json

Import("env")

def load_json_env(path=".env.json"):
    if not os.path.exists(path):
        return
    
    with open(path) as f:
        config = json.load(f)
        for key, value in config.items():
            
            print(f"[ENV] Set {key} from {path}")
            env.Append(BUILD_FLAGS=[f'''-D {key}='"{value}"' '''])

load_json_env()
load_json_env('env.json')
load_json_env('.secret/.env.json')
