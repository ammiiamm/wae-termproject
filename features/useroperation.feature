Feature: User Operation
  In order to start using our site, as an admin,
  I want to add site information.
  Scenario: Start adding site information
    Given I am an admin
    And I am signed in
    When I visit the main page
    Then I should see a my username at the top right page
    And I should see an image for clicking to site management page
    When I go to site management page
    Then I should see New site link
    And I click New site link
    When I visit the new site page
    Then I should be able to fill in new site information
    And I can click submit information

