import json
import requests
import os
import time

REFRESH_INTERVAL = 3 # seconds

with open(f'{os.environ["HOME"]}/.reddit_creds') as f:
    lines = f.readlines()
    CLIENTID = lines[0].strip()
    SECRET = lines[1].strip()
    USER = lines[2].strip()
    PW = lines[3].strip()

# taken from https://towardsdatascience.com/how-to-use-the-reddit-api-in-python-5e05ddfd1e5c
# while the token is valid (~2 hours) we just add headers=headers to requests
LAST_FETCHED = 0
REFETCH_INTERVAL = 4800 # seconds, 1.5h
LAST_HEADERS = None
def get_headers():
    global LAST_FETCHED, LAST_HEADERS
    if time.time() - LAST_FETCHED > REFETCH_INTERVAL:
        print('REFETCHING CREDS')
        auth = requests.auth.HTTPBasicAuth(CLIENTID, SECRET)
        data = {'grant_type': 'password',
                'username': USER,
                'password': PW}
        headers = {'User-Agent': 'yubo56_fetch/0.0.1'}
        res = requests.post('https://www.reddit.com/api/v1/access_token',
                            auth=auth, data=data, headers=headers)
        TOKEN = res.json()['access_token']
        LAST_HEADERS = {**headers, **{'Authorization': f'bearer {TOKEN}'}}
        LAST_FETCHED = time.time()
    return LAST_HEADERS

def parse_listing(listing):
    data = listing['data']
    return {
        'nawards': len(data['all_awardings']),
        'score': data['ups'] - data['downs'],
        'created': data['created'],
        'url': data['url'],
        # 'text': data['selftext'],
        'title': data['title'],
        'author': data['author'],
    }
def fetch_listings_from_mm():
    headers = get_headers()
    resp_obj = requests.get(
        'https://oauth.reddit.com/r/mechmarket/new', headers=headers).json()

    posts = [parse_listing(l) for l in resp_obj['data']['children']]
    return posts

def format_time(create_time):
    seconds_elapsed = time.time() - create_time
    if seconds_elapsed < 300:
        return '%ds' % seconds_elapsed
    return '%dm' % (seconds_elapsed // 60)
def print_listings(listings):
    os.system('clear')
    print('****************************************************************')
    for l in listings:
        print('[%d%s]{%s ago) %s {/u/%s}' % (
            l['score'],
            '*' * l['nawards'],
            format_time(l['created']),
            l['title'],
            l['author'],
        ))
    print('****************************************************************')

if __name__ == '__main__':
    while True:
        listings = fetch_listings_from_mm()
        print_listings(listings)
        time.sleep(REFRESH_INTERVAL)
