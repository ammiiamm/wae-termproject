class Ability
  include CanCan::Ability

  def initialize(user)

    user ||= User.new # guest user (not logged in)
    if user.admin?
      can :manage, :all
    elsif user.registered?
      can :read, HomeController
      can :read, WelcomeController
    elsif user.banned?
      can :read, HomeController
      can :read, WelcomeController
    elsif user.viewer?
      can :read, DashboardController
      can :read, HomeController
      can :read, WelcomeController
    elsif user.user?
      can :manage, DashboardController
      can :manage, Advertisement
      can :manage, Camera
      can :manage, Configuration
      can :manage, SelectorController
      can :manage, HomeController
      can :manage, WelcomeController
      can :manage, Zone
      can :read, Site
    elsif user.advertiser?
      can :manage, HomeController
      can :manage, WelcomeController
      can :manage, Advertisement
    else
      can :read, HomeController
    end

  end
end
#
#
# if user.admin?
#   can :manage, :all
# elsif user.member?
#   can :read, Quotation
#   can :create, Quotation
#   can :update, Quotation do |quotation|
#     quotation.try(:user) == user
#   end
#   can :destroy, Quotation do |quotation|
#     quotation.try(:user) == user
#   end else
#         can :read, Quotation
# end
