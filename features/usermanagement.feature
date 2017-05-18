Feature: User Management
  In order to keep track of who are using our web application,
  as an admin, I want to view user information and their roles.
  Scenario: Make sure about admin permission
  An admin can 1) see recently registered users, 2) ban a user
    Given I am an admin
    And I am signed in
    When I visit the main page
    Then I should see a my username at the top right page
    And I should see an image for clicking to user management page
    When I click the link to user administration pages
    Then I should see user information
    And I should see their role

