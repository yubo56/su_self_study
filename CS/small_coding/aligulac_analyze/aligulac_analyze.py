# initdb
# createuser --interactive # create superuser
# createuser aligulac
# createdb aligulac
# psql <superuser>
# > GRANT SELECT ON ALL TABLES IN SCHEMA public TO aligulac;
# psql aligulac < <PATH>/aligulac.sql
# psql -U aligulac aligulac
from collections import defaultdict
import datetime
import matplotlib.pyplot as plt
import psycopg2
from operator import itemgetter

MAX_PERIODS = 300 # 208 rows total

def get_mu_stats(db, rca, rcb, smooth=1, selectors={}):
    """
    Given two races ["P", "T", "Z"], returns
        { period_id => [rca wins, total games] }
    """
    def get_stats(race_a, race_b):
        cur = db.cursor()
        # JOIN so we drop all invalid references (seem to be none, gj aligulac)
        query_str = """
            SELECT period_id, SUM(sca), SUM(scb)
            FROM match
            JOIN period ON match.period_id = period.id
            %s
            WHERE rca = '%s' AND rcb = '%s'%s
            GROUP BY period_id
        """ % (
            selectors.get("JOIN", ""),
            race_a,
            race_b,
            selectors.get("WHERE", ""))
        cur.execute(query_str)
        return cur.fetchmany(MAX_PERIODS)
    a_v_b = get_stats(rca, rcb)
    b_v_a = get_stats(rcb, rca)

    # there"s probably some nice reduce() out there for this, oh well
    period_stats = defaultdict(lambda: [0, 0])
    for period, awins, bwins in a_v_b:
        for idx in range(smooth):
            period_stats[period - idx + smooth // 2][0] += awins
            period_stats[period - idx + smooth // 2][1] += awins + bwins
    for period, bwins, awins in b_v_a:
        for idx in range(smooth):
            period_stats[period - idx + smooth // 2][0] += awins
            period_stats[period - idx + smooth // 2][1] += awins + bwins
    return dict(period_stats)

if __name__ == "__main__":
    db = psycopg2.connect("dbname=aligulac user=aligulac")
    cur = db.cursor()

    # get list of recent biweekly periods
    cur.execute("SELECT id, start FROM period WHERE computed = true")
    periods = cur.fetchmany(MAX_PERIODS)
    def to_year_dec(p_idx):
        return (periods[int(p_idx)][1].toordinal()
            - datetime.date(2000, 1, 1).toordinal()) / 365.25 + 2000
    def get_winperc_trunc_smooth(wins_dict, smooth=1):
        """ get win percentage truncating boundaries w/ smoothing artifacts"""
        winpercs_arr = [(to_year_dec(i), wins_dict[i][0] / wins_dict[i][1])
            for i in range(min(wins_dict.keys()) + smooth // 2,
                max(wins_dict.keys()) - smooth // 2)
            if i in wins_dict] # conditional checks before evaluation
        return zip(*sorted(winpercs_arr, key=itemgetter(0)))

    tvz_yrs, tvz_winpercs = get_winperc_trunc_smooth(
        get_mu_stats(db, "P", "Z", smooth=1),
        smooth=1)
    krtvz_yrs, krtvz_winpercs = get_winperc_trunc_smooth(
        get_mu_stats(db, "P", "Z", smooth=1, selectors={
            "JOIN": """ JOIN player pa ON match.pla_id = pa.id
                JOIN player pb ON match.plb_id = pb.id """,
            "WHERE": "AND pa.country = 'KR' AND pb.country = 'KR'" }), smooth=1)
    plt.plot(tvz_yrs, tvz_winpercs, 'k', label='All')
    plt.plot(krtvz_yrs, krtvz_winpercs, 'r', label='KR vs KR')
    plt.ylim((0.4, 0.6))
    plt.xticks(range(int(min(tvz_yrs)), int(max(krtvz_yrs) + 1)))
    plt.legend()
    plt.title('PvZ')
    plt.xlabel('Time Period (biweekly)')
    plt.ylabel('Win Rate')
    plt.show()
