class AddLongToSite < ActiveRecord::Migration
  def change
    add_column :sites, :Long, :string
  end
end
