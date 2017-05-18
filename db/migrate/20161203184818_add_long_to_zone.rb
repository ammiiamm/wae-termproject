class AddLongToZone < ActiveRecord::Migration
  def change
    add_column :zones, :Long, :string
  end
end
