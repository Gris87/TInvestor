import re

text_buy = "Покупка"
text_sale = "Продажа"
text_trade_interrupted = "Торговля прервана"
text_want_to_sell = "Решено продать"

huge_bid_regexp = re.compile(r"^Решено начать перепродажу, потому что количество продавцов больше в .* раза")
huge_spread_regexp = re.compile(r"^Решено начать перепродажу, потому что спред .*")
