-- Write a SQL query to list the names of all people who starred in a movie released in 2004, ordered by birth year
SELECT name FROM people
WHERE id IN (
    SELECT DISTINCT stars.person_id
    FROM stars JOIN movies ON movies.id = stars.movie_id
    WHERE year = 2004
)
ORDER BY birth ASC;