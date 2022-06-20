# Frank Lewis
# IT-140 TextBasedGame.py - Crystal Dragon Adventures

import random
import time
import os


# function to properly clear the output buffer depending on OS
def cls():
    os.system('cls' if os.name == 'nt' else 'clear')
    print('')


# intractable tools inside rooms 'name':{'recipe of items joined by ','':'resulting item received'}
# contains used for processing as well as examine information for the player
tools = {'furnace': {'sand,silver pieces': 'mirror', 'contains': []}}

# enemies that could be found in any room, their list value contains the item effective in defeating them followed by their perished verb
enemies = {'little demon': ['sword', 'slay'], 'salamander': ['sword', 'flatten'],
           'warlock': ['sword', 'deflect the spell and defeat'],
           'witch': ['mirror', 'hold the mirror up to the witch, trapping'],
           'dragon': ['dragonbinder', 'oops']}

#some random enemies, for potential leveling system for future projects as well as rewarding items that may have been missed.
random_enemies = {'shade': ['sand','disperse'], 'giant rat':['sword','pummel']}

# text that is displayed when the user examines a specific object
examine_text = {
    'dragonbinder': 'Used to bind the soul of the user with the soul of the dragon..\n'
                    'Some say breaking a dragonbinder in the presence of a dragon,\n'
                    'will allow it to be free once again.',
    'nest': "it seems to be asking for a shiny offering...",
    'hole': "a conspicous hole, perhaps you need something to lower yourself down?",
    'sand': "this may come in handy later",
    'sword': "I'm sure this could come in handy, at least it is very shiny",
    'silver pieces': "This can't be the gold the village lost... perhaps it will come in handy.",
    'mirror': "You probably don't want to look into a silver mirror...",
    'furnace': "You can make things in this furnace... try using items on it.",
    'pit': "a conspicous hole, perhaps you need something to lower yourself down?",
    'shade': "a dark abborition, you will need to hit it with something fine enough to disperse it",
    'witch': "a witch must never be shown their real form, or risk turning to stone."
}

# room definitions containing various bits of information
# name = room name
# tools = list of tool objects located in the room
# items = list of items in the room
# keys = list of items required to enter the room
# enemies = list of enemies in the room
# interactive = key:list value pair for what is interactive and the items it is looking for
# solved = items that have been correctly interacted with will be moved to this list where they can then be used see solved_commands and solved_actions below
# locked = items the player is unable to leave the room with in their inventory
# loot = extra items rewarded to the player for clearing all enemies
# TODO:add flavor text to rooms... to display as player enters without examine
rooms = [{
    '0': {'name': 'Hallway',
          'tools': [],
          'hint': 'A dark corridor, I should continue searching',
          'items': ['sand'],
          'locked': [],
          'keys': [],
          'enemies': [],
          'interactive': {},
          'solved': {}},
    '1': {'name': 'Grand Entrance',
          'tools': [],
          'hint': 'Perhaps I should explore around',
          'items': [],
          'locked': [],
          'keys': [],
          'enemies': [],
          'interactive': {},
          'solved': {}},
    '2': {'name': 'Study',
          'tools': [],
          'hint': 'These creatures don\'t look so friendly',
          'items': ['candle'],
          'locked': [],
          'keys': ['sword'],
          'enemies': ['little demon', 'salamander'],
          'interactive': {},
          'solved': {}},
    '3': {'name': 'Damp Cellar',
          'tools': [],
          'items': ['rope'],
          'locked':[],
          'keys': ['candle'],
          'enemies': [],
          'interactive': {},
          'solved': {}},
    '4': {'name': 'Pit Entrance',
          'interactive': {'pit|hole': ['rope']},
          'solved': {},
          'locked': [],
          'items': ['silver pieces'],
          'keys': [],
          'hint': 'You see a \'hole\' in the ground, \nperhaps you could lower yourself down somehow.',
          'enemies': [],
          'tools': []},
    '5': {'name': 'Dragon Cave',
          'hint': 'There is a \'furnace\' in this room',
          'items': [],
          'tools': ['furnace'],
          'keys': ['sand', 'silver pieces'],
          'enemies': ['warlock'],
          'interactive': {},
          'solved': {},
          'locked': ['sand', 'silver pieces']},
    '6': {'name': 'Empty Nest',
          'items': [],
          'keys': [],
          'locked': ['sword'],
          'enemies': [],
          'interactive': {'nest': ['sword']},
          'solved': {},
          'hint': 'You see shiny objects in the \'nest\', almost like an offering...\nPerhaps I should offer something also.',
          'tools': []},
    '7': {'name': 'Control Room',
          'items': [],
          'loot': ['dragonbinder'],
          'keys': ['mirror'],
          'enemies': ['witch'],
          'interactive': {},
          'solved': {},
          'locked': [],
          'tools': []},
    '8': {'name': 'Dragon\'s Lair',
          'loot': ['Holy Grail'],
          'hint': "The dragon seems miserable, like something is holding him here.",
          'keys': ['dragonbinder'],
          'enemies': ['dragon'],
          'interactive': {},
          'solved': {},
          'locked': ['dragonbinder'],
          'tools': [],
          'items': []},
    '9': {'name': 'Cave Exit'},
    's': {'name': 'Hallway',
          'tools': [],
          'items': ['sword'],
          'keys': [],
          'enemies': [],
          'interactive': {},
          'solved': {},
          'locked': []}
},{'0':{'name':"Village Street",
        'tools':[],
        'items':[],
        'keys':[],
        'enemies':[],
        'interactive':{}, #add stuff to use in town
        'solved': {},
        'locked': []},
   '1':{'name':"Bobs Bookshop",
        'hint':"Bob dabbles in magic, he is considering a teaching career",
        'tools':[],
        'items':[],
        'keys':[],
        'enemies':[],
        'interactive':{}, #bob to buy/learn
        'solved':{},
        'locked':[]},
   '2':{'name':"Shirleys supply co",
        'hint':"Shirley sells stuff...",
        'tools':[],
        'items':[],
        'keys':[],
        'enemies':[],
        'interactive':{},
        'solved':{},
        'locked':[]},
   '3':{'name':"Home Sweet Home",
        'tools':[],
        'items':[],
        'keys':[],
        'enemies':[],
        'interactive':{}, #storage locker to be added
        'solved':{}, #possible easteregg for small benefit
        'locked':[]},
   '4':{'name':"Freds Fine Forge",
        'hint':"Fred will make weapons and armor for you.",
        'tools':[],
        'items':[],
        'keys':[],
        'enemies':[],
        'interactive':{}, #allow interacting with fred
        'solved':{},
        'locked':[]},
   '5':{'name':"Forest entrance",
        'hint':"The next journey begins soon.", #continuation
        'tools':[],
        'items':[],
        'keys':[],
        'enemies':[],
        'interactive':{},
        'solved':{},
        'locked':[]}
}
]

# x,y direction effect lookup for cardinal direction commands
directions = {'n': ['y', -1], 'e': ['x', 1], 's': ['y', 1], 'w': ['x', -1]}

# cardinal abbreviations to full name for output
cardinals = {'n': 'North', 'e': 'East', 's': 'South', 'w': 'West'}

# the layout of the level
# . = wall
# 0 = hallway
# number = room
# other = special
towns = [1]

game_map = [ #level 0
            '..........' \
            '.....0008.' \
            '.000070.9.' \
            '.6.5...0..' \
            '.0.0.300..' \
            '....00.0..' \
            '..04s020..' \
            '..0.0.00..' \
            '....1.0...' \
            '..........',
             #level 1
            '..........' \
            '..........' \
            '.1..2.....' \
            '.0..0.....' \
            '.00000003.' \
            '..0..0....' \
            '..4..0005.' \
            '..........' \
            '..........' \
            '..........'
           ]

# game world dimensions
width = 10
height = 10

# empty list of ' ' used to reprisent the map displayed for the character.
traveled = [' '] * (width * height)

# player x/y coordinates
coords = {'x': 4, 'y': 8}

# area to hold various global information... player inventory, weather the game has ended and the players last position
flags = {'last_command':[],'level':0, 'player_inventory': [], 'game_over': 0, 'last_pos': {'l':0,'x': 4, 'y': 8}}

#determine if the room gets a random enemy

# used to move the player to a new location they otherwise could not get to
def transport_player(tele_coords, msg = ""):
    # if player somehow gets moved to a location they are not allowed in we need the last position to be able to move them back
    flags.update({'last_pos': {'l':flags['level'],'x': coords['x'], 'y': coords['y']}})
    coords['x'] = tele_coords[0]
    coords['y'] = tele_coords[1]

    flags['level'] = tele_coords[2]

    if len(msg) > 0:
      cls()
      return msg
    else:
      return "You suddenly find yourself in a new location"


# used to provide the player with an item, they did not have to pickup
def give(items):
    for item in items:
        flags['player_inventory'].append(item)
    return "You received a secret: {}".format(",".join(items))


# used to lookup the commands carried out by player interracting with solved objects,
# any commands added must be defined above this line
solved_commands = {'transport': transport_player, 'give': give}

# these are the keys for solved items in rooms format for key is "RoomNumberItem"
# values contain the name of the command to lookup and the arguments wanting to pass to it as a list
solved_actions = {'4pit|hole': ['transport', 3, 3,0], '6nest': ['give', 'pile of gold coins']}


# used to carry out player movement commands
def move_player(card):
    card_l = card[0]  # first character is used for lookup n,e,s,w
    if card_l in directions:
        pos = (coords['x'] + coords['y'] * width)
        # check for items in the players inventory that lock them to the room they are in
        if rooms[flags['level']][game_map[flags['level']][pos]]['locked']:
            if any(x in flags['player_inventory'] for x in rooms[flags['level']][game_map[flags['level']][pos]]['locked']):
                return "It seem this room will not allow you to leave with an item you have."

        # use a copy of the new coordinates to test that they are moving to a valid location
        new_coords = coords.copy()
        new_coords[directions[card_l][0]] += directions[card_l][1]
        new_pos = new_coords['x'] + new_coords['y'] * width
        if game_map[flags['level']][new_pos] == '.':
            return "Woa, You tried to go through a wall! OUCH!"
        if len(rooms[flags['level']][game_map[flags['level']][pos]]['enemies']) > 0:
            return "You try to leave the room but {} wont let you!\nYou must fight or attempt to run!".format(
                rooms[flags['level']][game_map[flags['level']][pos]]['enemies'][0])
        else:
            flags.update({'last_pos': {'l':flags['level'], 'x': coords['x'], 'y': coords['y']}})
            coords.update(new_coords)
            random_encounter = random.randint(0,10)
            if random_encounter < 2 and flags['level'] not in towns and random_enemies[list(random_enemies)[random_encounter % len(random_enemies)]][0] in flags['player_inventory']:
              rooms[flags['level']][game_map[flags['level']][(coords['x'] + coords['y'] * width)]]['enemies'].append(list(random_enemies)[random_encounter % len(random_enemies)])
            return "You move {}".format(cardinals[card] if len(card) == 1 else card.capitalize())
    else:
        return "The go/walk command accepts a direction. Try North,South,East or West\nEG: go n, go north\nShorthand commands for moving are also available,\nEG:n, e, s, w "


# called when player gives quit command
def quit_game(_notused=None):
    flags['game_over'] = True
    return "Sorry to see you go..."


# used to process picking up items in rooms when player uses take command
# TODO: add ability to remove items placed in furnace/tools
def take_item(item='take'):
    if item not in ['take', 't']:
        # test that they are trying to pickup a valid item in the current room
        if item in rooms[flags['level']][game_map[flags['level']][(coords['x'] + coords['y'] * width)]]['items']:
            flags['player_inventory'].append(item)
            rooms[flags['level']][game_map[flags['level']][(coords['x'] + coords['y'] * width)]]['items'].remove(item)

            # a little nod to zelda...
            if item == 'sword':
                return "It's dangerous to go alone, take this!"

            return "You place the {} into your inventory.".format(item)
        else:
            return "Hmmmm I do not see any {} in this room.".format(item)
    else:
        return "The {} command requires an item/object you would like to pick up.\nEG:{} bow, {} arrows".format(item, item, item)


# used to process player run command when they are attempting to run away from enemies
def run_away(_notused):
    # get list of enemies in the room
    danger = rooms[flags['level']][game_map[flags['level']][(coords['x'] + coords['y'] * width)]]['enemies']

    # calculate success chance and either escape or be caught by random enemy
    chance = random.randint(0, 10)
    if len(danger) > 0:
        if chance > 2:
            coords.update(flags['last_pos'])
            return "You managed to escape!"
        else:
            if len([k for k in danger if k in random_enemies.keys()]) > 0:
              return transport_player([4,8,0], "You tripped and fell while trying to run, the {} caught you!\nYou Find yourself at the entrance".format(danger[chance % len(danger)]))
            else:
              flags['game_over'] = True
              return "You tripped and fell while trying to run, the {} caught you!".format(danger[chance % len(danger)])
    else:
        # no enemies were in the room they tried to run out of
        return "I see no reason to run away from here."


# used to process player examine command if blank or 'room' it will display the rooms 'hint' if it has one.
# if a tool is examined it will return its recipes and contents, if there is an explicit examine text this is returned
# if examining an interactive object the player is told to use something with it, if examining something solved..
# the player is told to use the object.. otherwise nothing special to see.
def examine(what='x'):
    if what not in ['x', 'examine'] and what != 'room':
        solved_matches = [k for k in rooms[flags['level']][game_map[flags['level']][coords['x'] + coords['y'] * width]]['solved'].keys() if what in k]
        if what in tools.keys():
            return "The {} has the following uses:\n{}".format(what, "\n".join(["{} = {}".format(" + ".join(rec.split(",")), res) for rec, res in tools[what].items()]))
        elif solved_matches:
            return "Ahhh seems you might want to try to \'use {}\'".format(what)
        elif what in examine_text.keys():
            return examine_text[what]
        elif what in rooms[flags['level']][game_map[flags['level']][coords['x'] + coords['y'] * width]]['interactive'].keys():
            return "Seems I should use something with the {}".format(what)
        else:
            return "It is just a {}.".format(what)
    else:
        items_in_room = "This room contains some items!" if rooms[flags['level']][game_map[flags['level']][coords['x'] + coords['y'] * width]]['items'] else ""
        enemies_in_room = "Ill have to deal with these enemies!" if rooms[flags['level']][game_map[flags['level']][coords['x'] + coords['y'] * width]]['enemies'] else ""

        if what in ['x', 'examine']:
            examine('room')
            #return "The {} command is used to display additional info about the room, items, other object.\nEG:{} room".format(what, what)

        if 'hint' in rooms[flags['level']][game_map[flags['level']][coords['x'] + coords['y'] * width]].keys():
            return "{}{}{}".format((items_in_room + "\n" if items_in_room else ""),
                                   (enemies_in_room + "\n" if enemies_in_room else ""),
                                   rooms[flags['level']][game_map[flags['level']][coords['x'] + coords['y'] * width]]['hint'])

        else:
            if items_in_room or enemies_in_room:
                return "{}{}".format(items_in_room, enemies_in_room)
            else:
                return "Nothing special to see here"


def attack(item, target, room):
    if (target in enemies and enemies[target][0] == item) or (target in random_enemies and random_enemies[target][0] == item):  # check that the item we are using is useful against the enemy
        if random.randint(0, 10) < 7:  # random chance to miss
            rooms[flags['level']][room]['enemies'].remove(target)
            if 'loot' in rooms[flags['level']][room] and rooms[flags['level']][room]['enemies'] == []:  # drop loot
                rooms[flags['level']][room]['items'] += rooms[flags['level']][room]['loot']
            if target == 'dragon':  # player used dragonbinder on dragon... game over.. you lose..
                flags['game_over'] = True
                return "Your soul is bound to the dragon's and you both perish.\nFor some reason... I don't think I was meant to do that..."
            return "You {} the {}{}".format(enemies[target][1] if target in enemies else random_enemies[target][1], target,
                                            " and they dropped some loot!" if 'loot' in rooms[flags['level']][room] and rooms[flags['level']][room]['loot'] else "")
        else:
            if target in random_enemies.keys() and random.randint(0, 10) <= 3:
              transport_player([4,8,0])
              return "You missed with your {}, the {} caught you!\nYou Find yourself at the entrance".format(item,target)

            return "You swing your {} and MISSED!".format(item)
    else:
        return "This item seems ineffective against {}".format(target)

def show_help(_unused):
    return "Commands: go/walk/g/w <direction> or simply [n,e,s,w]\n          examine/x [room/object]\n"\
           "          use/u <item> on <object/enemy>\n          break <item>, take/t <item>, run/flee\n          r - repeat last command"

def interact(item, target, room):
    if item in rooms[flags['level']][room]['interactive'][target]:
        del rooms[flags['level']][room]['interactive'][target]
        flags['player_inventory'].remove(item)

        if item in rooms[flags['level']][room]['keys']:
            rooms[flags['level']][room]['keys'].remove(item)

        rooms[flags['level']][room]['solved'].update({target: "{}{}".format(room, target)})
        return "Used {} with {}, maybe I should use {}".format(item, target.split("|")[0], target.split("|")[0])
    else:
        return "That seems to do nothing."


def use_tool(item, target, room):
    if any(item in x for x in tools[target].keys()):
        tools[target]['contains'].append(item)
        flags['player_inventory'].remove(item)

        if item in rooms[flags['level']][room]['keys']:  # no longer require items removed from inventory for room access
            rooms[flags['level']][room]['keys'].remove(item)

        # check if a recipe is being completed in a tool (in forward/reverse order), if so...
        # empty tool and provide player with the results.
        in_tool = tools[target]['contains']
        alt_order_in_tool = in_tool[::-1]

        # TODO:update furnace to contain crafted item needing to be taken
        if ",".join(in_tool) in tools[target].keys() or ",".join(alt_order_in_tool) in tools[target].keys():
            craft = tools[target][",".join(in_tool)] if ",".join(in_tool) in tools[target].keys() else tools[target][",".join(alt_order_in_tool)]
            flags['player_inventory'].append(craft)
            tools[target]['contains'] = []
            return "You have created a {}!".format(craft)

        return "You put {} into the {}.".format(item, target)
    else:
        return "I don't think I want to put {} in {}".format(item, target)


# this is a rather robust function used to process the use command from the player
# this function has to be able to no only ignore invalid uses.. but handle attacking
# interacting, and using solved items
def use(item='use', target=None):
    if item not in ['use', 'u']:
        room = game_map[flags['level']][coords['x'] + coords['y'] * width]
        if not target:  # must be trying to use a solved interactable
            solved_matches = [k for k in rooms[flags['level']][room]['solved'].keys() if item in k]
            if solved_matches:
                s = solved_actions[rooms[flags['level']][room]['solved'][solved_matches[0]]]
                return solved_commands[s[0]](s[1:])
            else:
                return "Hmmmm I can't seem to use {}.{}".format(item, ("" if len(rooms[flags['level']][room]['interactive']) == 0 else " At least not yet..."))
        else:
            if item not in flags['player_inventory']:  # must be trying to use an item check that they have it
                return "You do not have {}".format(item)
            else:
                interact_item_matches = [k for k in rooms[flags['level']][room]['interactive'].keys() if target in k]

                if target in enemies.keys() and target in rooms[flags['level']][room]['enemies']:  # check if target is an enemy
                    return attack(item, target, room)
                elif interact_item_matches:  # check for interactive object that we may be using an item on
                    return interact(item, interact_item_matches[0], room)
                elif target in tools and target in rooms[flags['level']][room]['tools']:  # check for player using an item on a tool in the room
                    return use_tool(item, target, room)
                elif target in random_enemies.keys() and target in rooms[flags['level']][room]['enemies']:
                    return attack(item, target, room)
                else:
                    return "Either there are no {} in here or I don't know how a {} would be effective.".format(target, item)
    else:
        return "The {} command requires and item and object or enemy. \nEG: {} sword on rat, {} bucket with water".format(item, item, item)


# used at this time only for the win condition of the game (breaking the dragonbinder in the dragon's lair
def break_item(item='break'):
    if item != 'break':
        if item in flags['player_inventory']:
            if item == 'dragonbinder':
                room_num = game_map[flags['level']][coords['x'] + coords['y'] * width]
                if room_num == '8':  # if in dragon's lair free the dragon, remove the dragonbinder, and drop the holy grail.. also update room keys/enemies
                    rooms[flags['level']][room_num]['items'].append('holy grail')
                    flags['player_inventory'].remove(item)
                    rooms[flags['level']][room_num]['keys'].remove(item)
                    rooms[flags['level']][room_num]['enemies'].remove('dragon')
                    return "You break the dragonbinder, freeing the dragon!\n As you watch the dragon fly away, you notice something shiny on the ground."
                else:
                    return "I may need this in the future..."
            else:
                return "Not sure why I would break that."
        else:
            return "Trying to break something that is not yours I see..."
    else:
        return "The break command requires an item to break. \nWill break breakable items in the players inventory."


# used to lookup and call the corresponding commands for player commands any keys/values
# added should have functions defined above this line. allows for multiple text triggers
# for the same command IE:localization IE:go and walk, quit and exit, run and flee
commands = {'go': move_player, 'take': take_item, 'use': use, 'walk': move_player,
            'quit': quit_game, 'exit': quit_game, 'examine': examine, 'break': break_item,
            'run': run_away, 'flee': run_away, 'x': examine, 'n': move_player, 'e': move_player,
            's': move_player, 'w': move_player, 't': take_item, 'u': use, 'q': quit_game, 
            'h': show_help, 'help': show_help, 'c':show_help, 'commands':show_help}


def check_win_lose(room):
    if room['name'] == "Cave Exit":
        if all(x in flags['player_inventory'] for x in
               ['holy grail', 'pile of gold coins']):  # best ending... has the holy grail and the gold coins from the nest, puts player in main village (no ending gameplay or continuation yet)
            flags['player_inventory'].remove('pile of gold coins')
            print(transport_player([4,4,1], "You have successfully traversed the Dragon Cave, and returned the coins to the town.\nFor this you have been rewarded the holy grail!\nThe citizens no longer fear the cave, and the world is great again!"))
            
        elif 'holy grail' in flags['player_inventory']:  # player correctly freed the dragon and found the exit... however the towsfolk keep the grail...
            print(
                "You found your way through the caverns, you freed the dragon, and returned the holy grail to the town.\nThe citizens are happy yet you feel unrewarded for your efforts.")
            flags['game_over'] = True
        else:  # freed the dragon... missed picking up the grail and missed the nest interaction
            print(
                "You found your way through the caverns, you freed the dragon, and escaped back to civilization.\nHowever the citizens feel you may have forgot something...")
            flags['game_over'] = True
        return True
    else:
        return False


# prints the info for the room we are in
def print_room_info(pos, extra=""):
    if game_map[flags['level']][pos] != '.':  # should never happen... but player is in a wall
        room = rooms[flags['level']][game_map[flags['level']][pos]]

        # check for win/lose conditions
        if check_win_lose(room):
            return

        # used to move the player back to their previous position with a message as to why they could not enter yet...
        #TODO generalize this to account for all items required as listed, call messages from a map.
        if not all(x in flags['player_inventory'] for x in room['keys']):
            if 'candle' in room['keys']:
                print("Woa! It is much too dark to go into that room!")
            elif 'sword' in room['keys']:
                print("Woa! It is much too dangerous to go into that room unarmed!")
            else:
                print("Woa! It seems I am not prepared to go in that room!")
            traveled[coords['x'] + coords['y'] * 10] = ' '
            coords.update(flags['last_pos'])
            print_room_info(coords['x'] + coords['y'] * width, " Back")  # recursive call to describe the room we put them back into with the extra text "Back"
            return

        # print all room info and player inventory
        print("You are{} in the {}            Inventory:{}\n".format(extra, room['name'], (
            ", ".join(flags['player_inventory']) if len(flags['player_inventory']) > 0 else "Empty")))
        if len(room['enemies']) > 0:
            print("Enemies in room: {}".format(",".join(room['enemies'])))
        if len(room['items']) > 0:
            print("Items in room: {}".format(",".join(room['items'])))
    else:
        print("Not sure how to say this.. but you somehow fell out of the world.")


# used to determine which type of command and what format the player is attempting and call the correct command
def process_command(command):
    if command[0] == 'r':
      command = flags['last_command']
    else:
      flags['last_command'] = command

    if command[0] in commands:
        if len(command) < 2:  # single word command just gets looked up and called with the single word as a parameter (used for usage msg)
            return commands[command[0]](command[0])
        elif len(command) > 2:  # 3 or more word commands
            if command[0] in ['use', 'u']:  # check to see if multiple word item on an object or a single word item on a multiple word enemy
                if " ".join(command[1:3]) in flags['player_inventory']:
                    return commands[command[0]](" ".join(command[1:3]), " ".join(command[3:]))
                else:
                    return commands[command[0]](command[1], " ".join(command[2:]))
            else:  # likely not going to end up here.. however if they do... the command at worst will display failed message..
                return commands[command[0]](" ".join(command[1:]))
        else:  # 2 word commands... call the command with the second word as argument.
            return commands[command[0]](command[1])
    else:
        return "that command is not valid!"


# used to display a "where I have been" map to the player
def print_map():
    rows = [traveled[i:i + width] for i in range(0, width * height, width)]
    print("\n----MAP-----")
    print("|{}|".format("".join(rows[coords['y'] - 1])))
    print("|{}|".format("".join(rows[coords['y']])))
    print("|{}|".format("".join(rows[coords['y'] + 1])))
    print("------------")


# initial command to start a game.. this will update the random seed, print initial information, and set gameover flag to false
# then start the loop of showing info taking and processing commands
def begin_game():
    random.seed(int(time.time()))
    coords.update({'x': 4, 'y': 8})
    flags['game_over'] = False
    cls()

    print("Crystal Dragon Adventures")
    print("-----------------------------------------------------------------")
    print(
        'You have been tasked by the towns people to traverse the dragon caves,\nLocate their lost treasure, and free them from the dragon.\nGood Luck!')
    while not flags['game_over']:
        pos = coords['x'] + coords['y'] * width
        traveled[flags['last_pos']['x'] + flags['last_pos']['y'] * width] = '.'
        traveled[pos] = 'O'
        print_map()
        print("type \'help\' for a list of commands\n")
        print_room_info(pos)
        if not flags['game_over'] and flags['level'] == flags['last_pos']['l']:
            command = input("Enter your command:").lower().replace("on ", "").replace('with ', '').split()
            if command:
                clear_after = process_command(command)
                if len(clear_after) > 0:
                    cls()
                    print(clear_after)
                else:
                    cls()
            else:
                cls()
                print("The Quest Must Continue!")
        else:
          t = [' ' for f in traveled]
          traveled.clear()
          traveled.extend(t)
          flags.update({'last_pos': {'l':flags['level'],'x': coords['x'], 'y': coords['y']}})

    print("Game Over.")
    input("\nPress Enter to exit.")
    return 0


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    begin_game()
