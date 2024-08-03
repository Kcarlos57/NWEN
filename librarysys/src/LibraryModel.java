/*
 * LibraryModel.java
 * Author:
 * Created on:
 */

 import javax.swing.*; // Importing Swing components for GUI dialogs
 import java.sql.*;    // Importing SQL components for database connectivity
 
 public class LibraryModel {
 
     // For use in creating dialogs and making them modal
     private JFrame dialogParent; // Parent frame for dialogs
     private Connection conn;     // Connection object to the database
 
     // Constructor to initialize the model and establish a database connection
     public LibraryModel(JFrame parent, String userid, String password) {
         dialogParent = parent; // Set the parent frame for dialogs
         try {
             // Load PostgreSQL JDBC driver
             Class.forName("org.postgresql.Driver");
             // Construct the database URL
             String url = "jdbc:postgresql://localhost/" + userid + "_jdbc";
             // Establish a connection to the database using the provided credentials
             conn = DriverManager.getConnection(url, userid, password);
             System.out.println("Connected to the database successfully.");
         } catch (ClassNotFoundException | SQLException e) {
             // Display an error message dialog if the connection fails
             JOptionPane.showMessageDialog(dialogParent,
                                           "Database connection error: " + e.getMessage(),
                                           "Database Error",
                                           JOptionPane.ERROR_MESSAGE);
         }
     }
 
     // Method to look up a book by its ISBN
     public String bookLookup(int isbn) {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (Statement stmt = conn.createStatement();
              ResultSet rs = stmt.executeQuery("SELECT isbn, title, edition_no, numofcop, numleft FROM book WHERE isbn = " + isbn)) {
             if (rs.next()) { // If the book is found
                 // Append book details to the result
                 result.append("ISBN: ").append(rs.getInt("isbn"))
                       .append("\nTitle: ").append(rs.getString("title"))
                       .append("\nEdition Number: ").append(rs.getInt("edition_no"))
                       .append("\nNumber of Copies: ").append(rs.getInt("numofcop"))
                       .append("\nNumber Left: ").append(rs.getInt("numleft"));
             } else { // If the book is not found
                 result.append("No book found with ISBN: ").append(isbn);
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving book: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to show the entire book catalogue
     public String showCatalogue() {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (Statement stmt = conn.createStatement();
              ResultSet rs = stmt.executeQuery("SELECT isbn, title, edition_no, numofcop, numleft FROM book ORDER BY title")) {
             while (rs.next()) { // Iterate through the result set
                 // Append each book's details to the result
                 result.append("ISBN: ").append(rs.getInt("isbn"))
                       .append(", Title: ").append(rs.getString("title"))
                       .append(", Edition: ").append(rs.getInt("edition_no"))
                       .append(", Copies Total: ").append(rs.getInt("numofcop"))
                       .append(", Copies Available: ").append(rs.getInt("numleft")).append("\n");
             }
             if (result.length() == 0) { // If no books are found
                 result.append("No books found in the catalogue.");
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving catalogue: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to show all loaned books
     public String showLoanedBooks() {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (Statement stmt = conn.createStatement();
              ResultSet rs = stmt.executeQuery("SELECT b.isbn, b.title, cb.customerid, c.l_name, c.f_name, cb.duedate FROM cust_book cb JOIN book b ON cb.isbn = b.isbn JOIN customer c ON cb.customerid = c.customerid")) {
             while (rs.next()) { // Iterate through the result set
                 // Append each loaned book's details to the result
                 result.append("ISBN: ").append(rs.getInt("isbn"))
                       .append(", Title: ").append(rs.getString("title"))
                       .append(", Loaned To: ").append(rs.getString("f_name") + " " + rs.getString("l_name"))
                       .append(", Customer ID: ").append(rs.getInt("customerid"))
                       .append(", Due Date: ").append(rs.getDate("duedate")).append("\n");
             }
             if (result.length() == 0) { // If no books are loaned out
                 result.append("No books are currently loaned out.");
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving loaned books: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to show an author's details by their ID
     public String showAuthor(int authorID) {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (PreparedStatement stmt = conn.prepareStatement("SELECT authorid, name, surname FROM author WHERE authorid = ?")) {
             stmt.setInt(1, authorID); // Set the author ID parameter
             ResultSet rs = stmt.executeQuery();
             if (rs.next()) { // If the author is found
                 // Append author's details to the result
                 result.append("Author ID: ").append(rs.getInt("authorid"))
                       .append(", Name: ").append(rs.getString("name"))
                       .append(", Surname: ").append(rs.getString("surname"));
             } else { // If the author is not found
                 result.append("No author found with ID: ").append(authorID);
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving author: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to show all authors
     public String showAllAuthors() {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (Statement stmt = conn.createStatement();
              ResultSet rs = stmt.executeQuery("SELECT * FROM author")) {
             while (rs.next()) { // Iterate through the result set
                 // Append each author's details to the result
                 result.append("Author ID: ").append(rs.getInt("authorid"))
                       .append(", Name: ").append(rs.getString("name"))
                       .append(", Surname: ").append(rs.getString("surname")).append("\n");
             }
             if (result.length() == 0) { // If no authors are found
                 result.append("No authors found.");
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving authors: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to show a customer's details by their ID
     public String showCustomer(int customerID) {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (PreparedStatement stmt = conn.prepareStatement("SELECT customerid, l_name, f_name, city FROM customer WHERE customerid = ?")) {
             stmt.setInt(1, customerID); // Set the customer ID parameter
             ResultSet rs = stmt.executeQuery();
             if (rs.next()) { // If the customer is found
                 // Append customer's details to the result
                 result.append("Customer ID: ").append(rs.getInt("customerid"))
                       .append(", Last Name: ").append(rs.getString("l_name"))
                       .append(", First Name: ").append(rs.getString("f_name"))
                       .append(", City: ").append(rs.getString("city"));
             } else { // If the customer is not found
                 result.append("No customer found with ID: ").append(customerID);
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving customer: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to show all customers
     public String showAllCustomers() {
         StringBuilder result = new StringBuilder(); // StringBuilder to accumulate the result
         try (Statement stmt = conn.createStatement();
              ResultSet rs = stmt.executeQuery("SELECT * FROM customer")) {
             while (rs.next()) { // Iterate through the result set
                 // Append each customer's details to the result
                 result.append("Customer ID: ").append(rs.getInt("customerid"))
                       .append(", Last Name: ").append(rs.getString("l_name"))
                       .append(", First Name: ").append(rs.getString("f_name"))
                       .append(", City: ").append(rs.getString("city")).append("\n");
             }
             if (result.length() == 0) { // If no customers are found
                 result.append("No customers found.");
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error retrieving customers: " + e.getMessage();
         }
         return result.toString(); // Return the result as a string
     }
 
     // Method to borrow a book for a customer
     public String borrowBook(int isbn, int customerID, int day, int month, int year) {
         String result;
         try {
             // Disable auto-commit to handle transactions manually
             conn.setAutoCommit(false);
 
             // Check if the book is available
             PreparedStatement checkAvailabilityStmt = conn.prepareStatement("SELECT numleft FROM book WHERE isbn = ?");
             checkAvailabilityStmt.setInt(1, isbn);
             ResultSet rs = checkAvailabilityStmt.executeQuery();
             if (!rs.next() || rs.getInt("numleft") <= 0) {
                 conn.rollback(); // Roll back if the book is not available
                 return "Book is not available for borrowing.";
             }
 
             // Decrement the number of available copies
             PreparedStatement updateBookStmt = conn.prepareStatement("UPDATE book SET numleft = numleft - 1 WHERE isbn = ?");
             updateBookStmt.setInt(1, isbn);
             updateBookStmt.executeUpdate();
 
             // Record the borrowing transaction
             PreparedStatement borrowStmt = conn.prepareStatement("INSERT INTO cust_book (isbn, customerid, duedate) VALUES (?, ?, ?)");
             borrowStmt.setInt(1, isbn);
             borrowStmt.setInt(2, customerID);
             borrowStmt.setDate(3, Date.valueOf(String.format("%d-%02d-%02d", year, month, day))); // Format the due date
             borrowStmt.executeUpdate();
            JOptionPane.showMessageDialog(null, "Waiting");
             conn.commit(); // Commit the transaction
             result = "Book borrowed successfully.";
         } catch (SQLException e) {
             try {
                 conn.rollback(); // Roll back the transaction in case of an error
             } catch (SQLException ex) {
                 // Ignore the rollback exception
             }
             result = "Error borrowing book: " + e.getMessage();
         } finally {
             try {
                 conn.setAutoCommit(true); // Restore auto-commit mode
             } catch (SQLException e) {
                 // Ignore the exception
             }
         }
         return result; // Return the result message
     }
 
     // Method to return a borrowed book
     public String returnBook(int isbn, int customerID) {
         String result;
         try {
             // Disable auto-commit to handle transactions manually
             conn.setAutoCommit(false);
 
             // Check if the loan exists
             PreparedStatement checkLoanStmt = conn.prepareStatement("SELECT * FROM cust_book WHERE isbn = ? AND customerid = ?");
             checkLoanStmt.setInt(1, isbn);
             checkLoanStmt.setInt(2, customerID);
             ResultSet rs = checkLoanStmt.executeQuery();
             if (!rs.next()) {
                 conn.rollback(); // Roll back if the loan does not exist
                 return "No such loan found.";
             }
 
             // Delete the loan record
             PreparedStatement deleteLoanStmt = conn.prepareStatement("DELETE FROM cust_book WHERE isbn = ? AND customerid = ?");
             deleteLoanStmt.setInt(1, isbn);
             deleteLoanStmt.setInt(2, customerID);
             deleteLoanStmt.executeUpdate();
 
             // Increment the number of available copies
             PreparedStatement updateBookStmt = conn.prepareStatement("UPDATE book SET numleft = numleft + 1 WHERE isbn = ?");
             updateBookStmt.setInt(1, isbn);
             updateBookStmt.executeUpdate();
 
             conn.commit(); // Commit the transaction
             result = "Book returned successfully.";
         } catch (SQLException e) {
             try {
                 conn.rollback(); // Roll back the transaction in case of an error
             } catch (SQLException ex) {
                 // Ignore the rollback exception
             }
             result = "Error returning book: " + e.getMessage();
         } finally {
             try {
                 conn.setAutoCommit(true); // Restore auto-commit mode
             } catch (SQLException e) {
                 // Ignore the exception
             }
         }
         return result; // Return the result message
     }
 
     // Method to close the database connection
     public void closeDBConnection() {
         try {
             if (conn != null && !conn.isClosed()) {
                 conn.close(); // Close the connection
                 System.out.println("Database connection closed.");
             }
         } catch (SQLException e) {
             // Print an error message if a SQL exception occurs
             System.out.println("Error closing connection: " + e.getMessage());
         }
     }
 
     // Method to delete a customer by their ID
     public String deleteCus(int customerID) {
         String result;
         try (PreparedStatement stmt = conn.prepareStatement("DELETE FROM customer WHERE customerid = ?")) {
             stmt.setInt(1, customerID); // Set the customer ID parameter
             int affectedRows = stmt.executeUpdate();
             if (affectedRows > 0) {
                 result = "Customer deleted successfully.";
             } else {
                 result = "No customer found with ID: " + customerID;
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error deleting customer: " + e.getMessage();
         }
         return result; // Return the result message
     }
 
     // Method to delete an author by their ID
     public String deleteAuthor(int authorID) {
         String result;
         try (PreparedStatement stmt = conn.prepareStatement("DELETE FROM author WHERE authorid = ?")) {
             stmt.setInt(1, authorID); // Set the author ID parameter
             int affectedRows = stmt.executeUpdate();
             if (affectedRows > 0) {
                 result = "Author deleted successfully.";
             } else {
                 result = "No author found with ID: " + authorID;
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error deleting author: " + e.getMessage();
         }
         return result; // Return the result message
     }
 
     // Method to delete a book by its ISBN
     public String deleteBook(int isbn) {
         String result;
         try (PreparedStatement stmt = conn.prepareStatement("DELETE FROM book WHERE isbn = ?")) {
             stmt.setInt(1, isbn); // Set the ISBN parameter
             int affectedRows = stmt.executeUpdate();
             if (affectedRows > 0) {
                 result = "Book deleted successfully.";
             } else {
                 result = "No book found with ISBN: " + isbn;
             }
         } catch (SQLException e) {
             // Return an error message if a SQL exception occurs
             return "Error deleting book: " + e.getMessage();
         }
         return result; // Return the result message
     }
 }
 