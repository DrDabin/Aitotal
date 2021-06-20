## Aitotal

Данная программа предназначена для проверки файлов на зловреднось на сервисе https://www.virustotal.com.
В программе предусмотрено проверка как отдельных файлов так и архивов(архив распаковывается во временную папку).

Данная программа проверяет файлы на VirusTotal.com, и выводит результат. **Данная программа не заменяет антивирусы**.

Как пользоваться:
- Перенести файлы\папку на иконку.
- Перенести файлы\папку на запущенную программу.
- В меню Файл выбрать "Открыть файл" и "Открыть папку".
- В командной строке. "путь к программе\aitotal.exe" /scan "имя файла" или "путь к программе\aitotal.exe" /norescan "имя файла" если не надо перепроверять файл.

Для остановки проверки нужно нажать кнопку "Стоп".

Имеется настройка (Дополнительно-Настройки), сохраняются настройки в файл "tools/options.ini"

Настраивается:
- Количество загружаемых файлов (максимально количество 20). В файле настроек [Tools] FileCount=20 (не более 20).
- Общее количество проверяемых файлов (максимальное количество 50). В файле настроек [Tools] ThreadCount=50 (не более 50)
- Ключ Apikey. apikey=
- Действие при закрытие программы, Закрыть программу или Свернуть в Трей. В файле настроек [Tools] Exit_Trey= (0-Свернуть в трей, 1-Закрыть программу)
- Уведомление при ошибки распаковки архивов. В файле настроек [Tools] MessageErrorArchive=1 (0 не выводить, 1- выводить)
- Настройки прокси.
- Настройка запрет на запуск дубликатов программы, только в файле настроек [Tools] RunDuplicateApplication=0 (0-дубликат не создавать, но все данные переносить в основную форму, 1-разрешено создавать дубликат.)
- Вывод сообщения об окончании проверки файла [Tools] BallonHint=0 (0 не выводить, 1- выводить)

Возможности:
- Aitotal автоматически распаковывает архивы с популярными вирусными паролями. Если архив большой, то на время распаковки программа может зависать (после окончания распаковки продолжит нормальную работу).
- Понимает структуру карантинов AVZ, uVS, TDSSKiller - то есть можно прямо перетаскивать архив с карантином от данных программ и в программе вы будете видеть имена файлов, как они назывались у пользователя. Также поддерживаются карантины AntiSMS.
- Сохранение отчёта о проверке в CSV файл.
- Применение своих паролей. В папке tools\password.txt не более 10 штук.
- Добавлено ввод пароля в форме. Если ни один пароль не подошёл, то выводится форма для его ввода.

Для разархивирование использую исходный код 7-zip автор Igor Pavlov. Ссылка http://7-zip.org
