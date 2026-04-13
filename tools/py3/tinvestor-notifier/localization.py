import re

msg_operations_inactivity = "⚠ Внимание! На брокерском счёте давно не было транзакций. Проверьте работоспособность приложения. Возможно, потребуется продать акции вручную."
msg_core_file_found = "⚠ Внимание! Обнаружен сбой приложения. Пожалуйста, проведите анализ, используя созданный core файл."
msg_app_restart = "⚠ Внимание! Приложение было не запущено. Выполнен повторный запуск."
msg_huge_sell = "⚠ Внимание! Обнаружено резкое падение цены для {ticker}({name})"
msg_dividends = "⚠ Внимание! Объявлены дивиденды для {ticker}({name}) с доходностью {yield_value}%"
msg_pulse_text_found = "⚠ Внимание! Найден текст в посте Пульса: {found_text}"
msg_positions_changed = "Изменилось содержимое портфолио:"
msg_total_cost = "Стоимость портфеля: {total_cost:.2f} ₽"
msg_currency_cost = "{currency} = {cost:.2f} ₽"
msg_stock_cost = "{ticker}({quantity:.0f}) = {cost:.2f} ₽"

msg_recommend_to_investigate = "⚪ Может быть интересно"
msg_recommend_to_buy = "🟢 Возможно стоит покупать"
msg_recommend_to_short = "🔴 Возможно стоит брать в шорт/продавать"

recommend_to_investigate_regexp = re.compile(r".*(мсфо).*")
recommend_to_buy_regexp = re.compile(r".*(дивиденд.*(рекомендовал|выплатить)|(рекомендовал|выплатить).*дивиденд|отсечка).*")
recommend_to_short_regexp = re.compile(r".*(арест|задержа|банкротств| суд([^аноья]|$)| иск([^лору]|$)|изъят|сизо|упк рф|долг([^о]|$)|погиб|убит|умер([^е]|$)|уничтожен|взорван|взрыв|разрушен|допк|эмиссия).*")
