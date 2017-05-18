class User < ActiveRecord::Base
  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable and :omniauthable
  devise :database_authenticatable, :registerable,
         :rememberable, :trackable, :validatable
  belongs_to :role
  before_create :set_default_role

  def registered?
    self.role.name == "registered" if !self.role.blank?
  end

  def banned?
    self.role.name == "banned" if !self.role.blank?
  end

  def user?
    self.role.name == "user" if !self.role.blank?
  end

  def admin?
    self.role.name == "admin" if !self.role.blank?
  end

  def viewer?
    self.role.name == "viewer" if !self.role.blank?
  end

  def advertiser?
    self.role.name == "advertiser" if !self.role.blank?
  end

  private
  def set_default_role
    self.role ||= Role.find_by_name('registered')
  end
end
