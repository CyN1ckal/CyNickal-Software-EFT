import sqlite3
import requests
import json 

def run_query(query):
    headers = {"Content-Type": "application/json"}
    response = requests.post('https://api.tarkov.dev/graphql', headers=headers, json={'query': query})
    if response.status_code == 200:
        return response.json()
    else:
        raise Exception("Query failed to run by returning code of {}. {}".format(response.status_code, query))

ItemQuery = """
{
  items(lang: en) {
    id
    sellFor {
      priceRUB
      vendor {
        name
      }
    }
    shortName
  }
}
"""
def UpdateItemTable():
    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();
    result = run_query(ItemQuery);
    for Item in result['data']['items']:

        HighestTraderPrice = -1;
            
        for SellOption in Item['sellFor']:
            price = SellOption['priceRUB'];
            vendor = SellOption["vendor"]["name"];
            if(vendor == "Flea Market"):
                continue;

            if(price > HighestTraderPrice):
                HighestTraderPrice = price;

        cur.execute("INSERT OR IGNORE INTO item_data (bsg_id, short_name, trader_price) VALUES (?, ?, ?)", (Item['id'], Item['shortName'], HighestTraderPrice));

    con.commit();
    con.close();
    return;
UpdateItemTable()

QuestItemQuery = """
query MyQuery {
  questItems {
    name
    id
  }
}
"""
def UpdateQuestItemTable():
    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();
    result = run_query(QuestItemQuery);
    for Item in result['data']['questItems']:
        cur.execute("INSERT OR IGNORE INTO quest_item_data (bsg_id, quest_item_name) VALUES (?, ?)", (Item['id'], Item['name']));

    con.commit();
    con.close();
    return;
UpdateQuestItemTable()

ContainerQuery = """
{
  lootContainers {
    id
    name
  }
}
"""
def UpdateContainerTable():
    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();

    result = run_query(ContainerQuery);

    for Item in result['data']['lootContainers']:
       cur.execute("INSERT OR IGNORE INTO container_data (bsg_id, short_name) VALUES (?, ?)", (Item['id'], Item['name']));

    con.commit();
    con.close();
    return;

AmmoQuery ="""
{
  ammo {
    item {
      id
      shortName
    }
  }
}
"""
def UpdateAmmoTable():
    result = run_query(AmmoQuery);

    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();

    for Item in result['data']['ammo']:
         cur.execute("INSERT OR IGNORE INTO ammo_data (bsg_id, short_name) VALUES (?, ?)", (Item['item']['id'], Item['item']['shortName']));

    con.commit();
    con.close();
    return;
UpdateAmmoTable();

TaskQuery ="""
{
  tasks {
    id
    name
    objectives {
      id
      description
      type
      ... on TaskObjectiveBasic {
        zones {
          position {
            x
            y
            z
          }
          map {
            name
          }
        }
      }
      ... on TaskObjectiveQuestItem {
        zones {
          position {
            x
            y
            z
          }
          map {
            name
          }
        }
      }
      ... on TaskObjectiveMark {
        zones {
          position {
            x
            y
            z
          }
          map {
            name
          }
        }
      }
      ... on TaskObjectiveItem {
        zones {
          position {
            x
            y
            z
          }
          map {
            name
          }
        }
      }
    }
  }
}
"""
def UpdateTaskTable():
    result = run_query(TaskQuery);

    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();

    for Item in result['data']['tasks']:
         cur.execute("INSERT OR IGNORE INTO task_data (bsg_id, task_name) VALUES (?, ?)", (Item['id'], Item['name']));

    con.commit();
    con.close();
    return;
UpdateTaskTable();

def UpdateObjectiveTable():
    result = run_query(TaskQuery);

    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();

    for Task in result['data']['tasks']:

        for Objective in Task['objectives']:

            if("zones" in Objective and len(Objective['zones']) > 0):
                print(json.dumps(Objective['zones']));
                cur.execute("INSERT OR IGNORE INTO objective_data (bsg_id, objective_description, objective_type, owning_task, objective_zones) VALUES (?, ?, ?, ?, ?)", (Objective['id'], Objective['description'], Objective['type'], Task['id'], json.dumps(Objective['zones'])));
            else:
                cur.execute("INSERT OR IGNORE INTO objective_data (bsg_id, objective_description, objective_type, owning_task) VALUES (?, ?, ?, ?)", (Objective['id'], Objective['description'], Objective['type'], Task['id']));

    con.commit();
    con.close();
    return;
UpdateObjectiveTable()

ObjectiveItems = """
query MyQuery {
  tasks {
    id
    objectives {
      ... on TaskObjectiveItem {
        id
        items {
          id
        }
      }
    }
    name
  }
}
"""
def UpdateObjectiveItemTable():
    result = run_query(ObjectiveItems);
    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();
    for Task in result['data']['tasks']:
        for Objective in Task['objectives']:
            if('items' in Objective):
                cur.execute("UPDATE objective_data SET objective_items = ? WHERE bsg_id = ?", (json.dumps(Objective['items']), Objective['id']));
 
    con.commit();
    con.close();
    return;
UpdateObjectiveItemTable();

QuestItems = """
query MyQuery {
  tasks {
    id
    objectives {
      ... on TaskObjectiveQuestItem {
        id
        questItem {
          id
        }
      }
    }
    name
  }
}
"""
def UpdateQuestItems():
    result = run_query(QuestItems);
    con = sqlite3.connect('../CyNickal Software EFT/EFT_Data.db');
    cur = con.cursor();
    for Task in result['data']['tasks']:
        for Objective in Task['objectives']:
            if('questItem' in Objective):
                InsertString = "[" + json.dumps(Objective['questItem']) + "]"
                cur.execute("UPDATE objective_data SET objective_items = ? WHERE bsg_id = ?", (InsertString, Objective['id']));
 
    con.commit();
    con.close();
    return;

UpdateQuestItems();
