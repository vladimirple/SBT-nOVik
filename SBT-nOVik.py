import logging
import csv
import re
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity
import mysql.connector

# Настройка логирования
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Параметры подключения к базе данных
db_config = {
    'host': 'localhost',
    'user': 'root',
    'password': 'Buratinovova02',
    'database': 'bd',
    'charset': 'latin1'
}

# Путь к файлу CSV для сохранения результатов
filtered_csv_file_path = 'filtered_similar_definitions.csv'
stopwords_file_path = 'D:\stopwords.txt'

# Стоп-слова для исключения из определения при сравнении
ignore_stopwords = set([
    "и", "в", "во", "не", "что", "он", "на", "я", "с", "со", "как", "а", "то",
    "все", "она", "так", "его", "но", "да", "ты", "к", "у", "же", "вы", "за",
    "бы", "по", "только", "ее", "мне", "было", "вот", "от", "меня", "еще",
    "нет", "о", "из", "ему", "теперь", "когда", "даже", "ну", "вдруг", "ли",
    "если", "уже", "или", "ни", "быть", "был", "него", "до", "вас", "нибудь",
    "опять", "уж", "вам", "ведь", "там", "потом", "себя", "ничего", "ей",
    "может", "они", "тут", "где", "есть", "надо", "ней", "для", "мы", "тебя",
    "их", "чем", "была", "сам", "чтоб", "без", "будто", "чего", "раз", "тоже",
    "себе", "под", "будет", "ж", "тогда", "кто", "этот", "того", "потому",
    "этого", "какой", "совсем", "ним", "здесь", "этом", "один", "почти",
    "мой", "тем", "чтобы", "нее", "сейчас", "были", "куда", "зачем", "всех",
    "никогда", "можно", "при", "наконец", "два", "об", "другой", "хоть",
    "после", "над", "больше", "тот", "через", "эти", "нас", "про", "всего",
    "них", "какая", "много", "разве", "три", "эту", "моя", "впрочем", "хорошо",
    "свою", "этой", "перед", "иногда", "лучше", "чуть", "том", "нельзя",
    "такой", "им", "более", "всегда", "конечно", "всю", "между"
])

def get_database_connection(config):
    """Создание подключения к базе данных."""
    try:
        return mysql.connector.connect(**config)
    except mysql.connector.Error as err:
        logging.error(f"Ошибка подключения к базе данных: {err}")
        return None

def fetch_words(cursor, limit=20000):
    """Получение списка слов из базы данных с ограничением."""
    try:
        query = "SELECT page_title FROM page LIMIT %s;"
        cursor.execute(query, (limit,))
        return cursor.fetchall()
    except mysql.connector.Error as err:
        logging.error(f"Ошибка при выполнении запроса fetch_words: {err}")
        return []

def fetch_definitions(cursor, word):
    """Получение списка определений слова из базы данных."""
    try:
        query = """
            SELECT DISTINCT wiki_text.text 
            FROM page 
            JOIN lang_pos ON page.id = lang_pos.page_id 
            JOIN meaning ON lang_pos.id = meaning.lang_pos_id 
            JOIN wiki_text ON meaning.wiki_text_id = wiki_text.id 
            WHERE page.page_title = %s;
        """
        cursor.execute(query, (word,))
        return cursor.fetchall()
    except mysql.connector.Error as err:
        logging.error(f"Ошибка при выполнении запроса fetch_definitions: {err}")
        return []

def fetch_synonyms(cursor, word):
    """Получение списка синонимов для данного слова из базы данных."""
    try:
        query = """
            SELECT wiki_text.text 
            FROM meaning
            JOIN relation ON meaning.id = relation.meaning_id
            JOIN wiki_text ON relation.wiki_text_id = wiki_text.id
            JOIN lang_pos ON meaning.lang_pos_id = lang_pos.id
            JOIN page ON lang_pos.page_id = page.id
            WHERE relation.relation_type_id = 9 
            AND page.page_title = %s;
        """
        cursor.execute(query, (word,))
        return cursor.fetchall()
    except mysql.connector.Error as err:
        logging.error(f"Ошибка при выполнении запроса fetch_synonyms: {err}")
        return []

def preprocess_text(text, stopwords_to_ignore=None):
    """Удаление знаков препинания и приведение текста к нижнему регистру, удаление стоп-слов."""
    text = re.sub(r'[^\w\s]', '', text)  # Удаление знаков препинания
    words = text.lower().split()  # Приведение к нижнему регистру и разделение на слова
    if stopwords_to_ignore:
        words = [word for word in words if word not in stopwords_to_ignore]
    return ' '.join(words)

def load_stopwords(file_path):
    """Загрузка стоп-слов из файла."""
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            return set(file.read().splitlines())
    except Exception as e:
        logging.error(f"Ошибка при загрузке стоп-слов из файла: {e}")
        return set()

def find_similar_definitions_tfidf(definitions, stopwords, threshold=0.4):
    """Нахождение пар определений с наиболее высокой схожестью на основе TF-IDF."""
    # Преобразуем определения в список строк и предварительно обрабатываем их
    definition_texts = [preprocess_text(definition[1], ignore_stopwords) for definition in definitions]
    original_texts = [definition for definition in definitions]  # Сохраняем оригинальные тексты с привязкой к слову

    # Создаем TF-IDF векторизатор и преобразуем определения в TF-IDF матрицу
    vectorizer = TfidfVectorizer()
    tfidf_matrix = vectorizer.fit_transform(definition_texts)

    # Вычисляем косинусное сходство между всеми парами определений
    cosine_similarities = cosine_similarity(tfidf_matrix, tfidf_matrix)

    similar_pairs = set()  # Используем множество для автоматического удаления дубликатов

    for i in range(len(definition_texts)):
        for j in range(i + 1, len(definition_texts)):  # j начинается с i + 1, чтобы избежать повторного сравнения и сравнения с самим собой
            similarity = cosine_similarities[i][j]
            if similarity > threshold and similarity < 0.9:  # Исключаем пары с идентичными определениями (схожесть < 0.9)
                pair = tuple(sorted([original_texts[i], original_texts[j]]))  # Используем оригинальные тексты с привязкой к слову
                similar_pairs.add((pair[0], pair[1], similarity))

    # Фильтрация пар, содержащих стоп-слова
    filtered_pairs = [
        (def1, def2, sim) for def1, def2, sim in similar_pairs
        if not any(stopword in def1[1].lower() for stopword in stopwords) and not any(stopword in def2[1].lower() for stopword in stopwords)
    ]

    return filtered_pairs

def main():
    # Подключение к базе данных
    conn = get_database_connection(db_config)
    if conn is None:
        return

    try:
        with conn.cursor() as cursor:
            # Получение списка слов с ограничением
            words = fetch_words(cursor, limit=20000)

            if not words:
                logging.warning("Не удалось получить слова для обработки.")
                return

            # Список для хранения всех определений с привязкой к слову
            all_definitions = []

            # Получение определений и синонимов для каждого слова
            for word in words:
                word = word[0] # Преобразуем кортеж в строку и декодируем
                definitions = fetch_definitions(cursor, word)
                
                if not definitions:
                    logging.info(f"Нет определений для слова: {word}. Переход к следующему слову.")
                    continue

                synonyms = fetch_synonyms(cursor, word)

                if not synonyms:
                    logging.info(f"Нет синонимов для слова: {word}. Пропуск слова.")
                    continue
                #word = word[0].encode('latin1').decode('utf-8')
                # Добавление определений слова
                decoded_definitions = [(word, definition[0].encode('latin1').decode('utf-8')) for definition in definitions]
                all_definitions.extend(decoded_definitions)

                valid_synonym_found = False

                for synonym in synonyms:
                    synonym_decoded = synonym[0]
                    synonym_definitions = fetch_definitions(cursor, synonym_decoded)
                    if synonym_definitions:
                        valid_synonym_found = True
                        decoded_synonym_definitions = [(synonym_decoded, definition[0].encode('latin1').decode('utf-8')) for definition in synonym_definitions]
                        all_definitions.extend(decoded_synonym_definitions)
                    else:
                        logging.info(f"Нет определений для синонима: {synonym_decoded}. Пропуск синонима.")

                if not valid_synonym_found:
                    logging.info(f"Все синонимы для слова: {word} не имеют определений. Пропуск слова.")
                    continue

            if not all_definitions:
                logging.warning("Не удалось получить определения для обработки.")
                return

            # Загрузка стоп-слов
            stopwords = load_stopwords(stopwords_file_path)

            # Нахождение пар определений с наиболее высокой схожестью
            similar_pairs = find_similar_definitions_tfidf(all_definitions, stopwords, threshold=0.4)

            # Открытие CSV файла для записи
            with open(filtered_csv_file_path, 'w', newline='', encoding='utf-8') as csvfile:
                csv_writer = csv.writer(csvfile, delimiter='|')

                # Запись результатов в CSV файл
                for (word1, definition1), (word2, definition2), similarity in similar_pairs:
                    csv_writer.writerow([word1.encode('latin1').decode('utf-8'), definition1, word2.encode('latin1').decode('utf-8'), definition2, similarity])
                    #csv_writer.writerow([word2.encode('latin1').decode('utf-8'), definition2])
                    #csv_writer.writerow([similarity])  # Добавление значения схожести
                    csv_writer.writerow([])  # Пустая строка между парами
                    logging.info(f"Записана пара определений с высокой схожестью: {definition1} и {definition2} со схожестью {similarity}")

        logging.info(f"Пары определений с высокой схожестью успешно записаны в файл: {filtered_csv_file_path}")

    except mysql.connector.Error as err:
        logging.error(f"Ошибка при работе с базой данных: {err}")

    finally:
        # Закрытие подключения к базе данных
        if conn.is_connected():
            conn.close()
            logging.info("Соединение с базой данных закрыто.")

if __name__ == "__main__":
    main()
